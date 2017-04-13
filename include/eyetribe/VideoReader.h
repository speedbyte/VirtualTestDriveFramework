//
// Created by geislerd on 12.04.17.
//

#ifndef EYETRIBE_VIDEOREADER_H
#define EYETRIBE_VIDEOREADER_H

#include <libuvc/libuvc.h>
#include <core/Node.h>
#include <utils/Image.h>

#define EYETRIBE_VENDOR_ID 10667
#define EYETRIBE_PRODUCT_ID 251
#define EYETRIBE_SERIAL_NUMBER nullptr

#define EYETRIBE_DEFINE_FORMAT( id , width , height , fps ) \
    template <> class Format<id> : public TFormat<width,height,fps> {}

namespace saliency_sandbox {
    namespace eyetribe {

        template <uint32_t _width, uint32_t _height, uint32_t _fps>
        class TFormat {
        public:
            static const uint32_t WIDTH  = _width;
            static const uint32_t HEIGHT = _height;
            static const uint32_t FPS    = _fps;

            typedef saliency_sandbox::utils::_IntensityImage<_width,_height> Image;
        };

        template <uint32_t _format> class Format;
        EYETRIBE_DEFINE_FORMAT(0,1152,1536,27);
        EYETRIBE_DEFINE_FORMAT(1,1036,1536,30);
        EYETRIBE_DEFINE_FORMAT(2,768,1024,60);
        EYETRIBE_DEFINE_FORMAT(3,768,1536,42);
        EYETRIBE_DEFINE_FORMAT(4,2304,1536,27);
        EYETRIBE_DEFINE_FORMAT(5,2072,1536,30);
        EYETRIBE_DEFINE_FORMAT(6,1536,1024,60);
        EYETRIBE_DEFINE_FORMAT(7,1536,1536,42);

        template<uint32_t _format>
        class VideoReader : public saliency_sandbox::core::Node::
            template Input<>::
            template Output<typename Format<_format>::Image> {
        public:
            static const uint32_t WIDTH  = Format<_format>::WIDTH;
            static const uint32_t HEIGHT = Format<_format>::HEIGHT;
            static const uint32_t FPS    = Format<_format>::FPS;
            typedef typename Format<_format>::Image Image;

        private:
            uvc_context_t* m_uvc_context;
            uvc_device_t* m_uvc_device;
            uvc_device_t** m_uvc_device_list;
            uvc_device_handle_t* m_uvc_device_handle;
            uvc_stream_ctrl_t m_uvc_control;
            uvc_error_t m_uvc_error;

            uint32_t m_device;
            std::mutex m_mutex;
            cv::Mat2b m_yuvu;
            Image m_output;
            bool m_wait;
        public:
            VideoReader(uint32_t device);
            void capture(uvc_frame_t *frame);
            void calc() override;
            void reset() override;
        };
    }
}

#endif //EYETRIBE_VIDEOREADER_H
