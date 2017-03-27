//
// Created by geislerd on 24.03.17.
//

#include <kitti/EgoMotion.h>

namespace saliency_sandbox {
    namespace kitti {

        void FillConvexPoly( cv::Mat& img, const cv::Point2f* v, int npts, const void* color, int line_type, int shift);

        template<Camera _cam>
        EgoMotion<_cam>::EgoMotion() {
            this->template input<0>()->name("velodyne");
            this->template input<1>()->name("oxts");
            this->template input<2>()->name("calibration");
        }

        template<Camera _cam>
        Calibration *EgoMotion<_cam>::calibration() {
            return this->template input<2>()->value();
        };

        template<Camera _cam>
        cv::Matx44f EgoMotion<_cam>::oxts() {
            return cv::Matx44f((float *) this->template input<1>()->value()->data());
        };

        template<Camera _cam>
        VelodyneReader::Matrix *EgoMotion<_cam>::velodyne() {
            return this->template input<0>()->value();
        };

        template<Camera _cam>
        void EgoMotion<_cam>::calc() {
            Calibration *calibration;
            cv::Matx44f oxts;
            VelodyneReader::Matrix *velodyne;
            cv::Vec2f tmp2[2];
            cv::Vec3f tmp3[2], mot;
            cv::Vec4f p;
            cv::Mat2f flow;
            cv::Mat4f vel, velp;
            cv::Point c;
            std::vector<cv::Point2f> gt;

            // clear output
            this->flow(cv::Vec2f(0.0f, 0.0f));

            calibration = this->calibration();
            oxts = this->oxts();
            velodyne = this->velodyne();
            flow = this->flow();

            // calculate inverse motion
            p = (oxts * cv::Vec4f(0.0f, 0.0f, 0.0f, -0.1f));// / -10.0f;
            if(fabsf(p.val[0]) < FLT_EPSILON && fabsf(p.val[1]) < FLT_EPSILON && fabsf(p.val[2]) < FLT_EPSILON)
                return; // no motion -> no flow
            mot = cv::Vec3f(p.val);

            // bring it to velo world
            //calibration->imuDirToVeloDir(&mot, &mot, 1);
            calibration->imuToVelo(&mot,&mot,1);

            // remove 4th entry (reflection) from
            // velodyne output, since we use it as
            // far point now
            cv::multiply(velodyne->mat(), cv::Scalar(1.0f, 1.0f, 1.0f, 0.0f), vel);

            // set 1.0 as far point value
            cv::add(vel, cv::Scalar(0.0f, 0.0f, 0.0f, 1.0f), vel);

            // substract the mothion from oxts
            cv::subtract(vel, cv::Scalar(mot.val[0], mot.val[1], mot.val[2], 0.0f), velp);

            for (int i = 0; i < vel.rows; i++) {

                // calculate point positions
                tmp3[0] = cv::Vec3f(vel.template at<cv::Vec4f>(i).val);
                tmp3[1] = cv::Vec3f((oxts * vel.template at<cv::Vec4f>(i)).val);

                if (tmp3[0].val[0] < 0.0f || tmp3[1].val[0] < 0.0f)
                    continue; // skip behind the car

                // camera points
                calibration->veloToCam(tmp3, tmp2, 2, _cam);

                if (tmp2[1].val[0] < 0.0f || tmp2[1].val[0] > WIDTH - 1.0f || tmp2[1].val[1] < 0.0f ||
                    tmp2[1].val[1] > HEIGHT - 1.0f) {
                    continue; // point not in the image plane
                }

                tmp2[0] -= tmp2[1];

                c.x = (int)tmp2[1].val[0];
                c.y = (int)tmp2[1].val[1];

                flow.template at<cv::Vec2f>(c) = tmp2[0];
                gt.push_back(c);
            }

            if(this->properties()->template get<bool>("dense", true))
                this->denseInterp(gt);
        }

        template<Camera _cam>
        void EgoMotion<_cam>::denseInterp(std::vector<cv::Point2f>& points) {
            cv::Subdiv2D sub;
            std::vector<int> idx;
            std::vector<std::vector<cv::Point2f>> facets;
            std::vector<cv::Point2f> centers, *facet;
            cv::Point poly[1000], center;
            size_t szCenters, szFacet;
            cv::Vec2f f;
            cv::Mat2f flow;
            cv::Scalar fs;

            sub.initDelaunay(cv::Rect(0,0,WIDTH,HEIGHT));
            sub.insert(points);
            sub.getVoronoiFacetList(idx,facets,centers);

            szCenters = centers.size();
            flow = this->flow();

            for(size_t i = 0; i < szCenters; i++) {
                center = centers[i];
                facet = &(facets[i]);
                szFacet = MIN(facet->size(),1000);
                f = flow.template at<cv::Vec2f>(center);
                fs = cv::Scalar(f);

                //copy facet float points to int point array
                for(size_t j = 0; j < szFacet; j++)
                    poly[j] = facet->at(j);

                cv::fillConvexPoly(flow,poly,(int) szFacet,fs,8,0);
            }
        }

        template<Camera _cam>
        void EgoMotion<_cam>::reset() { }

        template class EgoMotion<LEFT_GRAY>;
        template class EgoMotion<RIGHT_GRAY>;
        template class EgoMotion<LEFT_RGB>;
        template class EgoMotion<RIGHT_RGB>;
    }
}