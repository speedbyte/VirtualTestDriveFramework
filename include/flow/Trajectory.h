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

        class Pose : public saliency_sandbox::utils::_Matrix<4,4,float> {

        };

        template<uint32_t _n>
        class Trajectory : public saliency_sandbox::utils::_Matrix<_n,1,cv::Vec4f> {

        };

        template<uint32_t _n>
        class ExtrapolatePose : public saliency_sandbox::core::Node::
        template Input<Pose>::
        template Output<Trajectory<_n>>{
        private:
            typedef boost::circular_buffer<std::pair<time_t,cv::Vec4f>> buf_t;

            Trajectory<_n> m_trajectory;
            buf_t m_buf;

        public:
            ExtrapolatePose() : m_buf(10) {
                this->template input<0>()->name("pose");
                this->template output<0>()->name("trajectory");
                this->template output<0>()->value(&(this->m_trajectory));

                this->reset();
            }

            cv::Matx44f oxts() {
                return cv::Matx44f((float*)this->template input<0>()->value()->data());
            };

            void calc() override {
                cv::Vec4f mot;
                cv::Matx44f oxts;
                tk::spline sx,sy;
                std::vector<double> vt, vx, vy;
                cv::Mat4f tr;
                double t;

                mot = cv::Vec4f(0.0f,0.0f,0.0f,1.0f);
                oxts = this->oxts();
                tr = this->m_trajectory.mat();

                mot = oxts * mot;
                this->m_buf.push_back(std::make_pair(this->time(),mot));

                // extract data series
                for(buf_t::iterator i = this->m_buf.begin(); i != this->m_buf.end(); i++) {
                    vt.push_back(double(i->first));
                    vx.push_back(double(i->second.val[0]));
                    vy.push_back(double(i->second.val[1]));
                }

                // add trailing 0-points
                vt.push_back(double(this->time()+_n*2));
                vx.push_back(0.0);
                vy.push_back(0.0);
                vt.push_back(double(this->time()+_n*2+1));
                vx.push_back(0.0);
                vy.push_back(0.0);

                // calculate spline
                sx.set_points(vt,vx);
                sy.set_points(vt,vy);

                // extrapolate trajectory
                mot = cv::Vec4f(0.0f,0.0f,0.0f,0.0f);
                for(int i = 0; i < _n; i++) {
                    t = double(this->time()+i);
                    mot += cv::Vec4f(float(sx(t))*(1.0f/(i/_n+1.0f)),float(sy(t))*(1.0f/(i/_n+1.0f)),0.0f,1.0f);
                    tr.template at<cv::Vec4f>(i) = mot;
                }
            }

            void reset() override {
                this->m_buf.clear();
            }
        };
    }
}

#endif //TRAJECTORY_H
