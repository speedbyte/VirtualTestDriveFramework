//
// Created by geislerd on 12.04.17.
//

#include <eyetribe/VideoReader.h>

namespace saliency_sandbox {
    namespace eyetribe {

        template<uint32_t _format>
        void cb(uvc_frame_t *frame, void *data);

        template<uint32_t _format>
        VideoReader<_format>::VideoReader(uint32_t device) : m_device(device), m_led_code(0x000f) {
            this->output()->name("image");
            this->template output<0>()->value(&this->m_output);

            this->reset();
        }

        template<uint32_t _format>
        VideoReader<_format>::~VideoReader() {
            uvc_stream_stop(this->m_uvc_stream_handle);
            uvc_stream_close(this->m_uvc_stream_handle);
            uvc_close(this->m_uvc_device_handle);
        }

        template<uint32_t _format>
        void VideoReader<_format>::calc() {
            float mean_a, mean_b, mean_d;
            uvc_frame* frame;

            this->m_uvc_error = uvc_stream_get_frame(this->m_uvc_stream_handle,&frame,-1);
            sserr << sscond(this->m_uvc_error < 0) << "error while grabbing frame: " << uvc_strerror(this->m_uvc_error) << ssthrow;

            if(frame == nullptr)
                return;

            if(frame->data_bytes == WIDTH*HEIGHT*sizeof(cv::Vec2b)) {
                cv::cvtColor(
                        cv::Mat2b(
                                HEIGHT, WIDTH,
                                (cv::Vec2b *) frame->data),
                        this->m_output.mat(),
                        CV_YUV2GRAY_YUYV,
                        CV_8UC1);
                cv::flip(this->m_output,this->m_output,1);
            }

            mean_a = this->properties()->template get<float>("mean",50);
            mean_b = float(cv::mean(this->m_output).val[0]);
            mean_d = mean_a - mean_b;

            if(mean_d < -3.0f) {
                uvc_get_exposure_abs(this->m_uvc_device_handle,&this->m_cur_exposure,UVC_GET_CUR);
                if(this->m_cur_exposure > this->m_min_exposure) {
                    this->m_cur_exposure--;
                    uvc_set_exposure_abs(this->m_uvc_device_handle,this->m_cur_exposure);
                }else {
                    uvc_get_gain(this->m_uvc_device_handle,&this->m_cur_gain,UVC_GET_CUR);
                    if(this->m_cur_gain > this->m_min_gain) {
                        this->m_cur_gain--;
                        uvc_set_gain(this->m_uvc_device_handle, this->m_cur_gain);
                    }
                }
            } else if(mean_d > 3.0f) {
                uvc_get_gain(this->m_uvc_device_handle,&this->m_cur_gain,UVC_GET_CUR);
                if(this->m_cur_gain < this->m_max_gain) {
                    this->m_cur_gain++;
                    uvc_set_gain(this->m_uvc_device_handle, this->m_cur_gain);
                } else {
                    uvc_get_exposure_abs(this->m_uvc_device_handle,&this->m_cur_exposure,UVC_GET_CUR);
                    if(this->m_cur_exposure < this->m_max_exposure) {
                        this->m_cur_exposure++;
                        uvc_set_exposure_abs(this->m_uvc_device_handle,this->m_cur_exposure);
                    }
                }
            }
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

            // open device
            this->m_uvc_error = uvc_open(this->m_uvc_device,&this->m_uvc_device_handle);
            sserr << sscond(this->m_uvc_error < 0) << "cannot open device (" << this->m_device << "): " << uvc_strerror(this->m_uvc_error) << ssthrow;

            // clear device list
            uvc_free_device_list(this->m_uvc_device_list,0);

            // setup format
            this->m_uvc_error = uvc_get_stream_ctrl_format_size(
                    this->m_uvc_device_handle,
                    &this->m_uvc_control,
                    UVC_FRAME_FORMAT_YUYV,
                    WIDTH,HEIGHT,FPS);
            sserr << sscond(this->m_uvc_error < 0)
                  << "cannot create stream control for device (" << this->m_device << "): "
                  << uvc_strerror(this->m_uvc_error) << ssthrow;

            // create stream
            this->m_uvc_error = uvc_stream_open_ctrl(this->m_uvc_device_handle,&this->m_uvc_stream_handle,&this->m_uvc_control);
            sserr << sscond(this->m_uvc_error < 0)
                  << "cannot open stream for device (" << this->m_device << "): "
                  << uvc_strerror(this->m_uvc_error) << ssthrow;

            this->m_uvc_error = uvc_stream_start(this->m_uvc_stream_handle, nullptr, nullptr,0);
            sserr << sscond(this->m_uvc_error < 0)
                  << "cannot start stream for device (" << this->m_device << "): "
                  << uvc_strerror(this->m_uvc_error) << ssthrow;

            // reverse engineered
            uvc_set_ctrl(this->m_uvc_device_handle,3,4,(void*)setup_data,8);

            uvc_get_gain(this->m_uvc_device_handle,&this->m_min_gain,UVC_GET_MIN);
            uvc_get_gain(this->m_uvc_device_handle,&this->m_max_gain,UVC_GET_MAX);
            uvc_get_gain(this->m_uvc_device_handle,&this->m_cur_gain,UVC_GET_CUR);

            uvc_get_exposure_abs(this->m_uvc_device_handle,&this->m_min_exposure,UVC_GET_MIN);
            uvc_get_exposure_abs(this->m_uvc_device_handle,&this->m_max_exposure,UVC_GET_MAX);
            uvc_get_exposure_abs(this->m_uvc_device_handle,&this->m_cur_exposure,UVC_GET_CUR);

            uvc_set_gain(this->m_uvc_device_handle, 50);
            uvc_set_exposure_abs(this->m_uvc_device_handle, 80);
            uvc_set_ctrl(this->m_uvc_device_handle,3,3,&this->m_led_code,2);
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