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

        class _VideoReaderImpl {
        private:
            cv::VideoCapture m_vc;
            boost::filesystem::path m_path;

        public:
            _VideoReaderImpl(boost::filesystem::path path);

            void reopen();

            bool read(cv::Mat3b& mat);
        };

        template <uint32_t _width, uint32_t _height>
        class _VideoReader : public _VideoReaderImpl, public core::Node::Input<>::Output<saliency_sandbox::utils::_RGBImage<_width,_height>> {
        private:
            utils::_RGBImage<_width,_height> m_rgb;

        public:
            _VideoReader(boost::filesystem::path path) : _VideoReaderImpl(path) {
                this->reset();
                this->template output<0>()->name("frame");
            }

            virtual void reset() override {
                this->reopen();
                this->output()->value(&(this->m_rgb));
            }

            virtual void calc() override {
                this->eof(!this->read(this->m_rgb));
            }
        };

        typedef _VideoReader<RESOLUTION> VideoReader;
    }
}

#endif //VIDEOREADER_H
