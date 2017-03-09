//
// Created by geislerd on 09.03.17.
//

#ifndef HIGHPASS_H
#define HIGHPASS_H

#include <saliency/Saliency.h>

namespace saliency_sandbox {
    namespace saliency {
        namespace activation {
            template<uint32_t _width, uint32_t _height>
            class _HighPass : public _Saliency<_width,_height,saliency_sandbox::utils::_HeatmapImage<_width,_height>> {
            private:
            public:

                float sigma() {
                    const float s = this->properties()->template get<float>("sigma",16.0f);

                    return s;
                }

                cv::Size filterSize() {
                    const int x = this->properties()->template get<int>("filter_size",21);

                    return cv::Size(x,x);
                }

                void calc() override {
                    cv::Mat1f smooth;
                    cv::Mat1f in;

                    in = this->template input<0>()->value()->mat();

                    cv::GaussianBlur(in,smooth,this->filterSize(),this->sigma(),this->sigma(),cv::BORDER_REPLICATE);
                    cv::absdiff(in,smooth,this->map());

                    this->normalize();
                }

                void reset() override {

                }
            };


            typedef _HighPass<RESOLUTION> HighPass;
        }
    }
}

#endif //HIGHPASS_H
