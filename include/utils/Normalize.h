//
// Created by geislerd on 12.03.17.
//

#ifndef NORMALIZE_H
#define NORMALIZE_H

#include <core/Node.h>
#include "Image.h"

namespace saliency_sandbox {
    namespace utils {
        template <uint32_t _width, uint32_t _height>
        class _Normalize : public saliency_sandbox::core::Node::template Input<_HeatmapImage<_width,_height>>::template Output<_HeatmapImage<_width,_height>> {
        private:
            _HeatmapImage<_width,_height> m_norm;

        public:
            _Normalize() {
                this->template input<0>()->name("matrix");
                this->template output<0>()->name("normalized");
                this->template output<0>()->value(&(this->m_norm));

                this->reset();
            }

            void calc() override {
                cv::Mat1f in, out;

                in = this->template input<0>()->value()->mat();
                out = this->template output<0>()->value()->mat();

                cv::normalize(in,out,0.0f,1.0f,cv::NORM_MINMAX);
            }

            void reset() override {

            }
        };

        typedef _Normalize<RESOLUTION> Normalize;
    }
}
#endif //NORMALIZE_H
