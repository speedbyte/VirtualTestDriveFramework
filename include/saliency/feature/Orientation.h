//
// Created by geislerd on 12.03.17.
//

#ifndef ORIENTATION_H
#define ORIENTATION_H

#include <saliency/Saliency.h>

namespace saliency_sandbox {
    namespace saliency {
        namespace feature {

            template<uint32_t _width, uint32_t _height>
            class _Orientation : public _Saliency<_width,_height,saliency_sandbox::utils::_HeatmapImage<_width,_height>> {
            public:

                float sigma() {
                    const float sigma = this->properties()->template get<float>("sigma",1.0f);

                    return sigma;
                }

                float theta() {
                    const float theta = this->properties()->template get<float>("theta",0.0f);

                    return theta;
                }

                float lambda() {
                    const float lambda = this->properties()->template get<float>("lambda",1.0f);

                    return lambda;
                }

                float gamma() {
                    const float gamma = this->properties()->template get<float>("gamma",1.0f);

                    return gamma;
                }

                void calc() override {
                    cv::Mat1f in, out, filter;

                    filter = cv::getGaborKernel(cv::Size(33,33),this->sigma(),this->theta(),this->lambda(),this->gamma());

                    in = this->template input<0>()->value()->mat();
                    out = this->map();

                    cv::filter2D(in,out,-1,filter,cv::Point(-1,-1),0,cv::BORDER_REPLICATE);
                }

                void reset() override {
                }
            };

            typedef _Orientation<RESOLUTION> Orientation;

        }
    }
}

#endif //ORIENTATION_H
