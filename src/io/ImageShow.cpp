//
// Created by geislerd on 08.02.17.
//

#include <io/ImageShow.h>

namespace saliency_sandbox {
    namespace io {

        ImageShow::ImageShow(std::string window) : core::Node::template Input<utils::IMatrix>::template Output<>(), m_window(window) {
            this->reset();

            this->template input<0>()->name("image");
            this->template input<0>()->checkPortSize(false);
        }

        ImageShow::~ImageShow() {
            if(this->properties()->get<bool>("close_window",true))
                cv::destroyWindow(this->m_window);
        }

        void ImageShow::calc() {
            if(this->template input<0>()->value()->bytes() > 0)
                cv::imshow(this->m_window, this->template input<0>()->value()->mat());
        }

        void ImageShow::reset() {

            cv::namedWindow(this->m_window, this->properties()->get("window_mode",(int) cv::WINDOW_AUTOSIZE));
            if(this->properties()->get<bool>("threaded",true))
                cv::startWindowThread();
            if(this->properties()->get("window_mode",(int) cv::WINDOW_AUTOSIZE) != cv::WINDOW_AUTOSIZE)
                cv::resizeWindow(this->m_window,this->properties()->get("width",512),this->properties()->get("height",512));
        }
    }
}