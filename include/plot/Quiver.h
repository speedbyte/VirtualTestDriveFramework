//
// Created by geislerd on 16.03.17.
//

#ifndef QUIVER_H
#define QUIVER_H

#include <flow/Flow.h>

namespace saliency_sandbox {
    namespace plot {

        template<uint32_t _width, uint32_t _height>
        class Quiver : public saliency_sandbox::core::Node::template Input<saliency_sandbox::flow::_FlowField<_width,_height>,saliency_sandbox::utils::_RGBImage<_width,_height>>::template Output<saliency_sandbox::utils::_RGBImage<_width,_height>> {
        private:
            saliency_sandbox::utils::_RGBImage<_width,_height> m_quiver;
        public:
            Quiver() {
                this->template input<0>()->name("flow");
                this->template input<1>()->name("rgb");

                this->template output<0>()->name("quiver");
                this->template output<0>()->value(&this->m_quiver);
            }

            void calc() override {
                cv::Mat2f flow;
                cv::Mat3b rgb, quiver;
                cv::Point2f v, vn, q, t;
                float angle, magnitude;
                int step;

                flow = this->template input<0>()->value()->mat();
                rgb = this->template input<1>()->value()->mat();
                step = this->properties()->template get<int>("step",20);

                this->m_quiver.mat(rgb);
                quiver = this->m_quiver.mat();

                for(cv::Point2f p(0,0); p.x < _width; p.x+=step) {
                    for(p.y = 0; p.y < _height; p.y+=step) {
                        v = flow.template at<cv::Vec2f>(p);

                        //if(fabsf(v.x) < FLT_EPSILON && fabsf(v.y) < FLT_EPSILON)
                        //    continue;

                        q = p + v;
                        magnitude = sqrtf(v.x*v.x+v.y+v.y);
                        vn = cv::Point2f(v.x/magnitude,v.y/magnitude);

                        cv::line(quiver, p, q, CV_RGB(255.0f*vn.x,255.0f*vn.y,255.0f), 1, CV_AA);
                    }
                }
            }

            void reset() override {

            }
        };
    }
}
#endif //QUIVER_H
