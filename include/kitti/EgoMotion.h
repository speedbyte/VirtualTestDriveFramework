//
// Created by geislerd on 16.03.17.
//

#ifndef EGOMOTION_H
#define EGOMOTION_H

#include <flow/Flow.h>
#include <kitti/VelodyneReader.h>
#include <kitti/OXTSReader.h>
#include <kitti/CalibrationReader.h>
#include <kitti/ImageReader.h>
#include <kitti/OXTSPose.h>

namespace saliency_sandbox {
    namespace kitti {

        template<Camera _cam>
        class EgoMotion : public saliency_sandbox::flow::_Flow<ImageReader<_cam>::Image::WIDTH, ImageReader<_cam>::Image::HEIGHT,VelodyneReader::Matrix,Pose,Calibration> {
        private:
            cv::Matx44f m_last;
        public:
            static const size_t WIDTH = ImageReader<_cam>::Image::WIDTH;
            static const size_t HEIGHT = ImageReader<_cam>::Image::HEIGHT;


            EgoMotion() {
                this->template input<0>()->name("velodyne");
                this->template input<1>()->name("oxts");
                this->template input<2>()->name("calibration");
            }

            Calibration* calibration() {
                return this->template input<2>()->value();
            };

            cv::Matx44f oxts() {
                return cv::Matx44f((float*)this->template input<1>()->value()->data());
            };

            VelodyneReader::Matrix* velodyne() {
                return this->template input<0>()->value();
            };

            void calc() override {
                Calibration* calibration;
                cv::Matx44f oxts;
                VelodyneReader::Matrix* velodyne;
                cv::Vec2f tmp2[2];
                cv::Vec3f tmp3[2], mot;
                cv::Vec4f p;
                cv::Mat2f flow;
                cv::Mat4f vel, velp;

                // clear output
                this->flow(cv::Vec2f(0.0f,0.0f));

                calibration = this->calibration();
                oxts = this->oxts();
                velodyne = this->velodyne();
                flow = this->flow();

                // calculate motion
                p = (oxts * cv::Vec4f(0.0f,0.0f,0.0f,1.0f))/-10.0f;
                mot = cv::Vec3f(p.val);

                // bring it to velo world
                calibration->imuDirToVeloDir(&mot,&mot,1);

                cv::multiply(velodyne->mat(),cv::Scalar(1.0f,1.0f,1.0f,0.0f),vel);
                cv::add(vel,cv::Scalar(0.0f,0.0f,0.0f,1.0f),vel);
                cv::subtract(vel,cv::Scalar(mot.val[0],mot.val[1],mot.val[2],0.0f),velp);

                for(int i = 0; i < vel.rows; i++) {

                    // calculate point positions
                    tmp3[0] = cv::Vec3f(vel.template at<cv::Vec4f>(i).val);
                    tmp3[1] = cv::Vec3f(velp.template at<cv::Vec4f>(i).val);

                    if(tmp3[0].val[0] < 0.0f || tmp3[1].val[0] < 0.0f) {
                        continue; // skip behind the car
                    }

                    // camera points
                    calibration->veloToCam(tmp3,tmp2,2,_cam);

                    if(tmp2[1].val[0] < 0.0f || tmp2[1].val[0] > WIDTH - 1.0f || tmp2[1].val[1] < 0.0f || tmp2[1].val[1] > HEIGHT - 1.0f) {
                        continue; // point not in the image plane
                    }

                    flow.template at<cv::Vec2f>((int)tmp2[1].val[1],(int)tmp2[1].val[0]) = tmp2[0]-tmp2[1];
                }

                this->m_last = oxts;
            }

            void reset() override {

            }
        };

        typedef EgoMotion<LEFT_GRAY> LeftGrayEgoMotion;
        typedef EgoMotion<RIGHT_GRAY> RightGrayEgoMotion;
        typedef EgoMotion<LEFT_RGB> LeftRGBEgoMotion;
        typedef EgoMotion<RIGHT_RGB> RightRGBEgoMotion;
    }
}

#endif //EGOMOTION_H
