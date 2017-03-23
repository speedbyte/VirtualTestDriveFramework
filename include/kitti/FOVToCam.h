//
// Created by geislerd on 23.03.17.
//

#ifndef FOVTOCAM_H
#define FOVTOCAM_H

#include <kitti/ImageReader.h>
#include <utils/Polar.h>

namespace saliency_sandbox {
    namespace kitti {

        template <Camera _cam>
        class FOVToCam : public saliency_sandbox::core::Node::
        template Input<saliency_sandbox::utils::PolarHeatmapImage,saliency_sandbox::utils::PolarHeatmapImage,saliency_sandbox::kitti::Calibration>::
        template Output<saliency_sandbox::utils::_HeatmapImage<saliency_sandbox::kitti::ImageReader<_cam>::Image::WIDTH,saliency_sandbox::kitti::ImageReader<_cam>::Image::HEIGHT>> {
        public:
            typedef saliency_sandbox::utils::_HeatmapImage<saliency_sandbox::kitti::ImageReader<_cam>::Image::WIDTH,saliency_sandbox::kitti::ImageReader<_cam>::Image::HEIGHT> Output;
        private:
            Output m_output;

        public:
            FOVToCam() {
                this->template input<0>()->name("fov");
                this->template input<1>()->name("depth");
                this->template input<2>()->name("calibration");

                this->template output<0>()->name("output");
                this->template output<0>()->value(&(this->m_output));
            }

            void poly(cv::Vec3f* p, int inclination, int azimuth, float r) {
                cv::Vec2f angle;

                angle.val[0] = ((inclination-0.5f)/180.0f)*float(M_PI);
                angle.val[1] = ((azimuth-0.5f)/180.0f)*float(M_PI);
                p[0].val[0] = r * sinf(angle.val[0]) * cosf(angle.val[1]);
                p[0].val[1] = r * sinf(angle.val[0]) * sinf(angle.val[1]);
                p[0].val[2] = r * cosf(angle.val[0]);

                angle.val[0] = ((inclination+0.5f)/180.0f)*float(M_PI);
                angle.val[1] = ((azimuth-0.5f)/180.0f)*float(M_PI);
                p[1].val[0] = r * sinf(angle.val[0]) * cosf(angle.val[1]);
                p[1].val[1] = r * sinf(angle.val[0]) * sinf(angle.val[1]);
                p[1].val[2] = r * cosf(angle.val[0]);

                angle.val[0] = ((inclination+0.5f)/180.0f)*float(M_PI);
                angle.val[1] = ((azimuth+0.5f)/180.0f)*float(M_PI);
                p[2].val[0] = r * sinf(angle.val[0]) * cosf(angle.val[1]);
                p[2].val[1] = r * sinf(angle.val[0]) * sinf(angle.val[1]);
                p[2].val[2] = r * cosf(angle.val[0]);

                angle.val[0] = ((inclination-0.5f)/180.0f)*float(M_PI);
                angle.val[1] = ((azimuth+0.5f)/180.0f)*float(M_PI);
                p[3].val[0] = r * sinf(angle.val[0]) * cosf(angle.val[1]);
                p[3].val[1] = r * sinf(angle.val[0]) * sinf(angle.val[1]);
                p[3].val[2] = r * cosf(angle.val[0]);
            }

            void calc() override {
                cv::Mat1f fov, depth, output;
                Calibration* calib;
                float v, r, dd;
                cv::Vec2f cam[4];
                cv::Vec3f p[4];
                cv::Point camp[4];

                fov = this->template input<0>()->value()->mat();
                depth = this->template input<1>()->value()->mat();
                calib = this->template input<2>()->value();
                output = this->m_output.mat();
                dd = this->properties()->template get<float>("default_depth",50.0f);

                this->m_output.clear();

                for(int i = 0; i < saliency_sandbox::utils::PolarHeatmapImage::WIDTH; i++) {// inclination
                    for (int j = 0; j < saliency_sandbox::utils::PolarHeatmapImage::HEIGHT; j++) {// inclination
                        v = fov.template at<float>(j,i); //fov
                        r = depth.template at<float>(j,i); //depth

                        if(v < FLT_EPSILON)
                            continue;

                        if(r < FLT_EPSILON)
                            r = dd;

                        this->poly(p,j,i,r);

                        if(p[0].val[0] < 0 || p[1].val[0] < 0 || p[2].val[0] < 0 || p[3].val[0] < 0)
                            continue; //skip points behind car

                        calib->veloToCam(p,cam,4,_cam);
                        camp[0].x = (int) round(cam[0].val[0]);
                        camp[0].y = (int) round(cam[0].val[1]);
                        camp[1].x = (int) round(cam[1].val[0]);
                        camp[1].y = (int) round(cam[1].val[1]);
                        camp[2].x = (int) round(cam[2].val[0]);
                        camp[2].y = (int) round(cam[2].val[1]);
                        camp[3].x = (int) round(cam[3].val[0]);
                        camp[3].y = (int) round(cam[3].val[1]);

                        if( (cam[0].val[0] >= 0.0f && cam[0].val[0] < Output::WIDTH - 1 && cam[0].val[1] >= 0.0f && cam[0].val[1] < Output::HEIGHT - 1) ||
                            (cam[1].val[0] >= 0.0f && cam[1].val[0] < Output::WIDTH - 1 && cam[1].val[1] >= 0.0f && cam[1].val[1] < Output::HEIGHT - 1) ||
                            (cam[2].val[0] >= 0.0f && cam[2].val[0] < Output::WIDTH - 1 && cam[2].val[1] >= 0.0f && cam[2].val[1] < Output::HEIGHT - 1) ||
                            (cam[3].val[0] >= 0.0f && cam[3].val[0] < Output::WIDTH - 1 && cam[3].val[1] >= 0.0f && cam[3].val[1] < Output::HEIGHT - 1))
                            cv::fillConvexPoly(output,camp,4,cv::Scalar(v),8,0);
                    }
                }
            }

            void reset() override {

            }
        };

    }
}

#endif //FOVTOCAM_H
