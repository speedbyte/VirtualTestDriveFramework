//
// Created by geislerd on 24.03.17.
//

#include <gaze/FOV.h>

namespace saliency_sandbox {
    namespace gaze {

        FOV::FOV() {
            // setup input ports
            this->template input<0>()->name("gaze");
            this->template input<1>()->name("depth");

            // setup output
            this->template output<0>()->name("fov");
            this->template output<0>()->value(&(this->m_fov));
        }

        float FOV::dot(cv::Vec3f x, cv::Vec3f y) {
            return x.val[0]*y.val[0]+x.val[1]*y.val[1]+x.val[2]*y.val[2];
        }

        float FOV::dist(cv::Vec3f x1, cv::Vec3f x2, cv::Vec3f x0) {
            cv::Vec3f x1x0, x2x1, x1x2;
            float x1x0n, x2x1n, x1x2n, x1x0x2x1d, d2, a;

            x1x0 = x1-x0;
            x1x2 = x1-x2;
            x2x1 = x2-x1;


            x1x0n = float(cv::norm(x1x0));
            x2x1n = float(cv::norm(x2x1));
            x1x2n = float(cv::norm(x1x2));

            a = acosf(this->dot(x1x0,x1x2)/(x1x0n*x1x2n));
            if(fabsf(a) > float(M_PI_2))
                return FLT_MAX;

            x1x0x2x1d = this->dot(x1x0,x2x1);

            d2 = (powf(x1x0n,2.0f)*powf(x2x1n,2.0f)-powf(x1x0x2x1d,2.0f))/powf(x2x1n,2.0f);
            d2 = sqrtf(fabsf(d2));

            return(d2);
        }

        float FOV::weight(float dist, float sigma) {
            float a, b, c, x;

            a = powf(sigma*sqrtf(2.0f*float(M_PI)),-1.0f);
            b = 0.0f;
            c = sigma;
            x = dist;

            return a*expf(-powf(x-b,2.0f)/(2.0f*powf(c,2.0f)));
        }

        void FOV::calc() {
            Gaze gaze(0.0f,0.0f,0.0f,0.0f,0.0f);
            cv::Mat1f depth, fov;
            cv::Vec3f origin, far, p;
            cv::Vec2f angle;
            float dist, r, weight, sigma, dd;
            double maxDepth;
            int maxDepthIdx, maxMaxDepthIdx;

            gaze = *this->template input<0>()->value();
            depth = this->template input<1>()->value()->mat();
            fov = this->m_fov.mat();
            sigma = this->properties()->template get<float>("sigma",0.1f);
            dd = this->properties()->template get<float>("default_depth",200.0f);

            // line origin
            origin = gaze.origin();
            // line far point
            far = origin + gaze.dir()*1000;

            for(int i = 0; i < saliency_sandbox::utils::PolarHeatmapImage::WIDTH; i++) {
                cv::minMaxIdx(depth.col(i), nullptr,&maxDepth, nullptr, &maxDepthIdx);
                if(maxDepth > 0.0f) {
                    dd = (float) maxDepth;
                    maxMaxDepthIdx = int(MAX(maxDepthIdx,saliency_sandbox::utils::PolarHeatmapImage::HEIGHT-maxDepthIdx));
                }
                for(int j = 0; j < saliency_sandbox::utils::PolarHeatmapImage::HEIGHT; j++) {
                    // cart coordinates of depth point
                    angle.val[0] = (j/180.0f)*float(M_PI); // inclination
                    angle.val[1] = (i/180.0f)*float(M_PI); // azimuth
                    r = depth.template at<float>(j,i);
                    // use default depth if we have no further information
                    if(r < FLT_EPSILON)
                        r = dd + dd*(fabsf(j-maxDepthIdx)/maxMaxDepthIdx);

                    p.val[0] = r * sinf(angle.val[0]) * cosf(angle.val[1]);
                    p.val[1] = r * sinf(angle.val[0]) * sinf(angle.val[1]);
                    p.val[2] = r * cosf(angle.val[0]);

                    // dist between gaze ray and depth point
                    dist = this->dist(origin, far, p);
                    // gaussian weight of distance
                    weight = this->weight(dist/r, sigma);
                    // write to output
                    fov.template at<float>(j, i) = weight;
                }
            }
        }

        void FOV::reset() { }

    }
}