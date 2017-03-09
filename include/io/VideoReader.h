//
// Created by geislerd on 07.02.17.
//

#ifndef VIDEOREADER_H
#define VIDEOREADER_H

#include <config.h>
#include <core/Node.h>
#include <utils/Image.h>
#include <utils/Resolution.h>
#include <boost/filesystem.hpp>

namespace saliency_sandbox {
    namespace io {
        template <uint32_t _width, uint32_t _height>
        class _VideoReader : public core::Node::Input<>::Output<utils::_RGBImage<_width,_height>> {
        private:
            utils::_RGBImage<_width,_height> m_rgb;
            cv::VideoCapture m_vc;
            boost::filesystem::path m_path;

        public:
            _VideoReader(boost::filesystem::path path) : core::Node::Input<>::Output<utils::_RGBImage<_width,_height>>(), m_vc(), m_path(path) {
                //TODO: check path
                this->reset();

                this->template output<0>()->name("frame");
            }

            virtual void reset() override {
                if(this->m_vc.isOpened())
                    this->m_vc.release();
                this->m_vc = cv::VideoCapture();
                this->m_vc.open(this->m_path.c_str());
            }

            virtual void calc() override {
                cv::Mat3b raw;

                this->m_vc >> raw;

                if(raw.size[0]>0 && raw.size[1]>0) {
                    cv::resize(raw, this->m_rgb, this->m_rgb.size());
                }
                else
                    ; //TODO: error handling - video reader returns empty mat

                this->output()->value(&(this->m_rgb));
            }
        };

        typedef _VideoReader<RESOLUTION> VideoReader;
    }
}

#endif //VIDEOREADER_H
