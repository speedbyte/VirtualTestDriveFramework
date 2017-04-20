//
// Created by geislerd on 12.04.17.
//

#include <eyetribe/VideoReader.h>

namespace saliency_sandbox {
    namespace eyetribe {

        template<uint32_t _format>
        void cb(uvc_frame_t *frame, void *data);

        template<uint32_t _format>
        VideoReader<_format>::VideoReader(uint32_t device) : m_device(device), m_wait(true) {
            this->output()->name("image");
            this->template output<0>()->value(&this->m_output);

            this->reset();
        }

        template<uint32_t _format>
        void VideoReader<_format>::capture(uvc_frame_t *frame) {
            if(frame->data_bytes != WIDTH*HEIGHT*sizeof(cv::Vec2b))
                return;
            this->m_mutex.lock();
            this->m_yuvu = cv::Mat2b(HEIGHT,WIDTH,(cv::Vec2b*)frame->data);
            cv::cvtColor(this->m_yuvu, this->m_output.mat(), CV_YUV2GRAY_YUYV, CV_8UC1);
            this->m_wait = false;
            this->m_mutex.unlock();
        }

        template<uint32_t _format>
        void VideoReader<_format>::calc() {
            uint16_t light_a,light_b;
            uint32_t exposure_a, exposure_b;
            uint16_t gain_a, gain_b;

            // turn on leds
            light_a = this->properties()->template get<bool>("infrared",true)?uint16_t(0x000f):uint16_t(0x0000);
            uvc_get_ctrl(this->m_uvc_device_handle,3,3,&light_b,2,UVC_GET_CUR);
            if(light_a != light_b)
                uvc_set_ctrl(this->m_uvc_device_handle,3,3,&light_a,2);

            // set exposure
            exposure_a = (uint32_t)this->properties()->template get<int>("exposure",120);
            uvc_get_exposure_abs(this->m_uvc_device_handle,&exposure_b,UVC_GET_CUR);
            if(exposure_a != exposure_b)
                uvc_set_exposure_abs(this->m_uvc_device_handle,exposure_a);

            // set gain
            gain_a = (uint16_t)this->properties()->template get<int>("gain",50);
            uvc_get_gain(this->m_uvc_device_handle,&gain_b,UVC_GET_CUR);
            if(gain_a != gain_b)
                uvc_set_gain(this->m_uvc_device_handle,gain_a);

            /*if(!this->m_wait) {
                this->m_mutex.lock();
                cv::cvtColor(this->m_yuvu, this->m_output.mat(), CV_YUV2GRAY_YUYV, CV_8UC1);
                this->m_mutex.unlock();
            }*/
            this->m_mutex.unlock();
            usleep(1);
            this->m_mutex.lock();
        }

        template<uint32_t _format>
        void VideoReader<_format>::reset() {
            int count;
            uint8_t setup_data[8];

            setup_data[0] = 250;
            setup_data[1] = 0;
            setup_data[2] = 240;
            setup_data[3] = 0;
            setup_data[4] = 250;
            setup_data[5] = 0;
            setup_data[6] = 240;
            setup_data[7] = 0;

            // initialize uvc context
            this->m_uvc_error = uvc_init(&this->m_uvc_context, nullptr);
            sserr << sscond(this->m_uvc_error < 0) << "cannot initialize libuvc: " << uvc_strerror(this->m_uvc_error) << ssthrow;

            // get all available devices
            this->m_uvc_error = uvc_find_devices(
                    this->m_uvc_context,
                    &this->m_uvc_device_list,
                    EYETRIBE_VENDOR_ID,
                    EYETRIBE_PRODUCT_ID,
                    EYETRIBE_SERIAL_NUMBER);
            sserr << sscond(this->m_uvc_error < 0) << "cannot retrieve device list: " << uvc_strerror(this->m_uvc_error) << ssthrow;

            // find device
            for(count = 0; this->m_uvc_device_list[count] != nullptr; count++);
            sserr << sscond(count <= this->m_device) << "device with index " << this->m_device << " not found" << ssthrow;
            this->m_uvc_device = this->m_uvc_device_list[this->m_device];

            // clear device list
            uvc_free_device_list(this->m_uvc_device_list,0);

            // open device
            this->m_uvc_error = uvc_open(this->m_uvc_device,&this->m_uvc_device_handle);
            sserr << sscond(this->m_uvc_error < 0) << "cannot open device (" << this->m_device << "): " << uvc_strerror(this->m_uvc_error) << ssthrow;

            // setup format
            this->m_uvc_error = uvc_get_stream_ctrl_format_size(
                    this->m_uvc_device_handle,
                    &this->m_uvc_control,
                    UVC_FRAME_FORMAT_YUYV,
                    WIDTH,HEIGHT,FPS);
            sserr << sscond(this->m_uvc_error < 0)
                  << "cannot create stream control for device (" << this->m_device << "): "
                  << uvc_strerror(this->m_uvc_error) << ssthrow;

            // reverse engineered
            uvc_set_ctrl(this->m_uvc_device_handle,3,4,(void*)setup_data,8);

            uvc_start_streaming(this->m_uvc_device_handle,&this->m_uvc_control,cb<_format>,this,0);
        }

        template<uint32_t _format>
        void cb(uvc_frame_t *frame, void *data) {
            ((VideoReader<_format>*)data)->capture(frame);
        }

        template class VideoReader<0>;
        template class VideoReader<1>;
        template class VideoReader<2>;
        template class VideoReader<3>;
        template class VideoReader<4>;
        template class VideoReader<5>;
        template class VideoReader<6>;
        template class VideoReader<7>;
    }
}