//
// Created by geislerd on 28.04.17.
//

#ifndef EYETRIBE_EYEBALLESTIMATION_H
#define EYETRIBE_EYEBALLESTIMATION_H

#include <eyetribe/StereoGlintMatcher.h>
#include "StereoGlintMatcher.h"

namespace saliency_sandbox {
    namespace eyetribe {
        template<uint32_t _format>
        class EyeballEstimation : public saliency_sandbox::core::Node::
        template Input<
                typename Format<_format>::Image,
                typename Format<_format>::Image,
                StereoGlint,
                StereoGlint,
                RectificationProjection,
                RectificationProjection
        >::template Output<> {
        public:

            void spherePoints(cv::Point3f center, float radius, std::vector<cv::Point3f>& points, std::vector<float>& vtheta, std::vector<float>& vphi, int steps) {
                const float step = float(M_PI) / steps;
                cv::Point3f p;

                for(float theta = step/4.0f; theta <= float(M_PI_2) + step / 2.0f; theta+=step) {
                    for (float phi = step/4.0f; phi <= 2.0f * float(M_PI) + step / 2.0f; phi += step) {
                        p.x = radius * sinf(theta) * cosf(phi);
                        p.y = radius * sinf(theta) * sinf(phi);
                        p.z = radius * cosf(theta);
                        p+=center;
                        points.push_back(p);
                        vtheta.push_back(theta);
                        vphi.push_back(phi);
                    }
                }
            }

            void drawSphere(cv::Mat3b& img, const std::vector<cv::Point2f>& points, const cv::Scalar_<double> color) {
                size_t sz;
                cv::Point2f p[2];

                sz = points.size();

                for(size_t i = 1; i < sz; i++) {
                    p[0] = points[i-1];
                    p[1] = points[i];
                    cv::line(img,p[0],p[1],color);
                }
            }

            void projection(RectificationProjection &P, std::vector<cv::Point3f> &v3d, std::vector<cv::Point2f> &v2d) {
                const cv::Matx34f p(P.mat());
                cv::Point3f a;

                for(std::vector<cv::Point3f>::iterator i = v3d.begin(); i != v3d.end(); ++i) {
                    a = p*cv::Vec4f(i->x,i->y,i->z,1.0);
                    v2d.push_back(cv::Point2f(a.x/a.z,a.y/a.z));
                }
            }

            void radon(std::vector<cv::Point2f>& v2d, cv::Mat1b& in, cv::Mat1b& radon) {
                int j = 0;
                cv::Point2f p;

                for(int x = 0; x < radon.cols; x++)
                    for(int y = 0; y < radon.rows; y++) {
                        j = x*radon.rows + y;
                        p = v2d[j];

                        if(p.x >= 0.0f && p.y >= 0.0f && p.x < in.rows && p.y < in.cols)
                            radon.template at<uchar>(y,x) = in.template at<uchar>(v2d[j]);
                    }
            }

            void calc() override {
                cv::Mat1b& in0 = *this->template input<0>()->value();
                cv::Mat1b& in1 = *this->template input<1>()->value();
                StereoGlint& leftGlint = *this->template input<2>()->value();
                StereoGlint& rightGlint = *this->template input<3>()->value();
                RectificationProjection& p0 = *this->template input<4>()->value();
                RectificationProjection& p1 = *this->template input<5>()->value();
                cv::Matx34d p[2];
                cv::Mat3b out0, out1;
                const float eyeSize = this->properties()->template get<float>("eye_size",18.0f);
                const int steps = this->properties()->template get<int>("steps",50);
                cv::Mat1b leftRadon[2], rightRadon[2];

                std::vector<float> theta, phi;
                std::vector<cv::Point3f> leftSphere3D, rightSphere3D;
                std::vector<cv::Point2f> leftSphere2D[2], rightSphere2D[2];

                this->spherePoints(leftGlint.world,eyeSize,leftSphere3D,theta,phi,steps);
                this->spherePoints(rightGlint.world,eyeSize,rightSphere3D,theta,phi,steps);

                this->projection(p0,leftSphere3D,leftSphere2D[0]);
                this->projection(p1,leftSphere3D,leftSphere2D[1]);
                this->projection(p0,rightSphere3D,rightSphere2D[0]);
                this->projection(p1,rightSphere3D,rightSphere2D[1]);

                leftRadon[0] = cv::Mat1b(steps,steps);
                leftRadon[1] = cv::Mat1b(steps,steps);
                rightRadon[0] = cv::Mat1b(steps,steps);
                rightRadon[1] = cv::Mat1b(steps,steps);

                this->radon(leftSphere2D[0],in0,leftRadon[0]);
                this->radon(leftSphere2D[1],in1,leftRadon[1]);
                this->radon(rightSphere2D[0],in0,rightRadon[0]);
                this->radon(rightSphere2D[1],in1,rightRadon[1]);

                cv::cvtColor(in0,out0,cv::COLOR_GRAY2RGB);
                cv::cvtColor(in1,out1,cv::COLOR_GRAY2RGB);


                this->drawSphere(out0,leftSphere2D[0],leftGlint.confidence > exp10f(-8.0f) ? cv::Scalar(255,0,0) : cv::Scalar(0,0,255));
                this->drawSphere(out0,rightSphere2D[0],rightGlint.confidence > exp10f(-8.0f) ? cv::Scalar(0,255,0) : cv::Scalar(0,0,255));
                this->drawSphere(out1,leftSphere2D[1],leftGlint.confidence > exp10f(-8.0f) ? cv::Scalar(255,0,0) : cv::Scalar(0,0,255));
                this->drawSphere(out1,rightSphere2D[1],rightGlint.confidence > exp10f(-8.0f) ? cv::Scalar(0,255,0) : cv::Scalar(0,0,255));

                cv::namedWindow("LEFT");
                cv::imshow("LEFT",out0);
                cv::namedWindow("RIGHT");
                cv::imshow("RIGHT",out1);

                cv::namedWindow("I0 LEFT");
                cv::imshow("I0 LEFT",leftRadon[0]);
                cv::namedWindow("I0 RIGHT");
                cv::imshow("I0 RIGHT",rightRadon[0]);

                cv::namedWindow("I1 LEFT");
                cv::imshow("I1 LEFT",leftRadon[1]);
                cv::namedWindow("I1 RIGHT");
                cv::imshow("I1 RIGHT",rightRadon[1]);
            }

            void reset() override {

            }

        };
    }
}
#endif //EYETRIBE_EYEBALLESTIMATION_H
