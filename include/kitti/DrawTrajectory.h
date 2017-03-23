//
// Created by geislerd on 17.03.17.
//

#ifndef DRAW_TRAJECTORY_H
#define DRAW_TRAJECTORY_H

#include <kitti/OXTSPose.h>

#include <kitti/ImageReader.h>
#include <flow/Trajectory.h>

namespace saliency_sandbox {
    namespace kitti {

        template<Camera _cam, size_t _n>
        class DrawTrajectory
                : public saliency_sandbox::core::Node::
                template Input<saliency_sandbox::flow::Trajectory<_n>,Calibration, typename ImageReader<_cam>::Image>::
                template Output<saliency_sandbox::utils::_RGBImage<ImageReader<_cam>::Image::WIDTH, ImageReader<_cam>::Image::HEIGHT>> {
        public:
            static const size_t WIDTH = ImageReader<_cam>::Image::WIDTH;
            static const size_t HEIGHT = ImageReader<_cam>::Image::HEIGHT;
        private:
            saliency_sandbox::utils::_RGBImage<WIDTH, HEIGHT> m_rgb;

            cv::Mat3b copyToRgb();

            Calibration* calibration() {
                return this->template input<1>()->value();
            };

            cv::Mat4f trajectory() {
                return this->template input<0>()->value()->mat();
            };

            cv::Mat3b copyToRgb(saliency_sandbox::kitti::LeftGrayImageReader::Image* value) {
                value->convert(&this->m_rgb);
                return this->m_rgb.mat();
            }

            cv::Mat3b copyToRgb(saliency_sandbox::kitti::LeftRGBImageReader::Image* value) {
                cv::Mat3b mat;
                mat = value->mat();
                this->m_rgb.mat(mat);
                return this->m_rgb.mat();
            }

        public:
            DrawTrajectory() {
                this->template input<0>()->name("trajectoy");
                this->template input<1>()->name("calibration");
                this->template input<2>()->name("scene");

                this->template output<0>()->name("rgb");
                this->template output<0>()->value(&this->m_rgb);
            }

            void calc() override {
                Calibration* calibration;
                cv::Mat4f tr;
                cv::Mat3b rgb;
                cv::Vec3f lr[_n+1], rr[_n+1];
                cv::Vec2f lc[_n+1], rc[_n+1];
                cv::Point p[4];
                cv::Vec4f v[2];
                cv::Matx44f TRl, TRr;
                std::stringstream s;

                calibration = this->calibration();
                tr = this->trajectory();
                rgb = this->copyToRgb(this->template input<2>()->value());

                TRl = calibration->imuToVeloTR();
                TRr = calibration->imuToVeloTR();

                lr[0] = cv::Vec3f(0.0f,1.1f,-1.73f);
                rr[0] = cv::Vec3f(0.0f,-1.1f,-1.73f);

                for(int i = 0; i < _n; i++) {
                    v[0] = TRl * tr.template at<cv::Vec4f>(i);
                    v[1] = TRr * tr.template at<cv::Vec4f>(i);

                    lr[i+1] = lr[0] + cv::Vec3f(v[0].val);
                    rr[i+1] = rr[0] + cv::Vec3f(v[1].val);

                    lr[i+1].val[2] = lr[0].val[2];
                    rr[i+1].val[2] = rr[0].val[2];
                }

                lr[0] = cv::Vec3f(1.0f,1.1f,-1.73f);
                rr[0] = cv::Vec3f(1.0f,-1.1f,-1.73f);

                calibration->veloToCam(lr,lc,_n+1,_cam);
                calibration->veloToCam(rr,rc,_n+1,_cam);

                for(int i = 1; i < _n+1; i++) {
                    if(lr[i-1].val[0] < 1.0f || rr[i-1].val[0] < 1.0f || lr[i].val[0] < 1.0f || rr[i].val[0] < 1.0f) {
                        continue;
                    }

                    p[0] = cv::Point(lc[i-1]);
                    p[1] = cv::Point(lc[i]);
                    p[2] = cv::Point(rc[i]);
                    p[3] = cv::Point(rc[i-1]);

                    for(int j = 1; j < 4; j++)
                        cv::line(rgb,p[j-1],p[j],CV_RGB(0,255,0));

                    if((cv::Point2f(rc[i-1]).y - p[2].y) > 10) {
                        s << i << "s";
                        cv::putText(rgb, s.str(), p[2], CV_FONT_HERSHEY_COMPLEX_SMALL, 0.6, cv::Scalar(255, 255, 255), 1, CV_AA);
                        s = std::stringstream();
                        s << (int)roundf(lr[i].val[0]) << "m";
                        cv::putText(rgb, s.str(), p[1], CV_FONT_HERSHEY_COMPLEX_SMALL, 0.6, cv::Scalar(255, 255, 255), 1, CV_AA);
                        s = std::stringstream();
                    }
                }
            }

            void reset() override {

            }
        };
    }
}

#endif //DRAW_TRAJECTORY_H
