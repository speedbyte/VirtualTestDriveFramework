//
// Created by geislerd on 28.03.17.
//

#ifndef IMAGEMEMORYREADER_H
#define IMAGEMEMORYREADER_H

#include <core/Node.h>
#include <opencv2/opencv.hpp>

namespace saliency_sandbox {
    namespace io {
        template <uint32_t _width, uint32_t _height, template<uint32_t,uint32_t> typename _image_type>
        class _ImageMemoryReader : public saliency_sandbox::core::Node::
        template Input<cv::Mat>::
        template Output<_image_type<_width,_height>> {
        public:
            typedef typename _image_type<_width,_height>::Mat Mat;
            typedef _image_type<_width,_height> Image;
        private:
            Image m_image;
        public:
            _ImageMemoryReader() {
                this->input()->name("image");
                this->input()->checkPortSize(false);

                this->output()->name("image");
                this->output()->value(&this->m_image);
            }

            void calc() override {
                cv::Mat* in;

                in = this->template input<0>()->value();
                cv::resize(*in, this->m_image, this->m_image.size());
            }

            void reset() override { }
        };
    }
}

#endif //IMAGEMEMORYREADER_H
