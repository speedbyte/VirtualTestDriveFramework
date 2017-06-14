//
// Created by geislerd on 18.03.17.
//

#ifndef TRAJECTORY_H
#define TRAJECTORY_H

#include <utils/Matrix.h>
#include <boost/circular_buffer.hpp>
#include <flow/Spline.h>

namespace saliency_sandbox {
    namespace flow {

        class Pose : public saliency_sandbox::utils::_Matrix<4,4,float> { };

        template<uint32_t _n>
        class Trajectory : public saliency_sandbox::utils::_Matrix<_n,1,cv::Vec4f> { };

        template<uint32_t _n>
        class ExtrapolatePose : public saliency_sandbox::core::Node::
        template Input<Pose>::
        template Output<Trajectory<_n>>{
        private:
            Trajectory<_n> m_tr;

        public:
            ExtrapolatePose() {
                this->template input<0>()->name("pose");
                this->template output<0>()->name("trajectory");
                this->template output<0>()->value(&(this->m_tr));

                this->reset();
            }

            cv::Matx44f oxts() {
                return cv::Matx44f((float*)this->template input<0>()->value()->data());
            };

            void calc() override {
                cv::Vec4f mot;
                cv::Matx44f oxts;

                oxts = this->oxts();
                mot = cv::Vec4f(0.0f,0.0f,0.0f,1.0f);

                // extrapolate trajectory
                for(int i = 0; i < _n; this->m_tr.template at<cv::Vec4f>(i++) = mot = oxts * mot);
            }

            void reset() override {
            }
        };
    }
}

#endif //TRAJECTORY_H
