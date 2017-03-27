//
// Created by geislerd on 27.03.17.
//

#ifndef DRAWBIRDVIEW_H
#define DRAWBIRDVIEW_H

#include <core/Node.h>
#include <flow/Trajectory.h>
#include <utils/Image.h>
#include <kitti/TrackletReader.h>
#include <gaze/FOV.h>
#include <boost/lexical_cast.hpp>
#include "VelodyneReader.h"

namespace saliency_sandbox {
    namespace kitti {

        typedef saliency_sandbox::utils::_RGBImage<1000,1000> BirdViewImage;

        template <uint32_t _n>
        class DrawBirdView : public saliency_sandbox::core::Node::
            template Input<
                saliency_sandbox::flow::Trajectory<_n>,          // <0> ego trajectory
                VelodyneReader::Matrix,                          // <1> velodyne data
                TrackletList,                                    // <2> tracklets
                saliency_sandbox::utils::PolarHeatmapImage,      // <3> fov
                saliency_sandbox::utils::PolarHeatmapImage       // <4> depth
            >::template Output<BirdViewImage> {
        private:
            BirdViewImage m_out;
            cv::Matx44f m_TR;

        public:
            DrawBirdView() : m_out(), m_TR(
                    0.0f,-10.0f,0.0f,this->m_out.rows/2.0f,
                    -10.0f,0.0f,0.0f,this->m_out.cols/1.15f,
                    0.0f,0.0f,0.0f,0.0f,
                    0.0f,0.0f,0.0f,1.0f) {
                this->template input<0>()->name("ego");
                this->template input<1>()->name("velodyne");
                this->template input<2>()->name("tracklets");
                this->template input<3>()->name("fov");
                this->template input<4>()->name("depth");

                this->template output<0>()->name("birdview");
                this->template output<0>()->value(&(this->m_out));
            }

            void objectPoints(cv::Vec4f points[9], float w, float l, cv::Matx44f TR = cv::Matx44f::eye()) {
                float _w = w / 2.0f;
                float _l = l / 2.0f;
                cv::Vec4f _points[9] = {
                        {-_l     ,-_w  ,0.0f,1.0f},
                        {+_l     ,-_w  ,0.0f,1.0f},
                        {-_l     ,+_w  ,0.0f,1.0f},
                        {+_l     ,+_w  ,0.0f,1.0f},
                        {+_l+1.0f,+0.0f,0.0f,1.0f},
                        {+_l     ,-_w  ,0.0f,1.0f},
                        {+_l     ,+_w  ,0.0f,1.0f},
                        {-_l     ,-_w  ,0.0f,1.0f},
                        {-_l     ,+_w  ,0.0f,1.0f}};
                cv::Matx44f _TR = this->m_TR * TR;

                for(int i = 0; i < 9; points[i] = (_TR * _points[i++]));
            }

            void drawObjectPoints(BirdViewImage::Mat& out, cv::Vec4f points[9], cv::Scalar color) {
                for(int i = 1; i < 9; i++)
                    cv::line(out,
                             cv::Point2f(points[i-1].val[0],points[i-1].val[1]),
                             cv::Point2f(points[i].val[0],points[i].val[1]),
                             color);
            }

            void drawEgoTrajectory(BirdViewImage::Mat& out) {
                cv::Vec4f egocar[9];
                saliency_sandbox::flow::Trajectory<_n>* t;
                cv::Vec4f t0, t1;

                t = this->template input<0>()->value();
                this->objectPoints(egocar,1.820f,4.774f);
                this->drawObjectPoints(out,egocar,cv::Scalar::all(255.0f));

                for(int i = 1; i < _n; i++) {
                    t0 = this->m_TR*t->template at<cv::Vec4f>(i-1);
                    t1 = this->m_TR*t->template at<cv::Vec4f>(i);
                    cv::line(out,
                             cv::Point2f(t0.val[0]-5.0f,t0.val[1]),
                             cv::Point2f(t1.val[0]-5.0f,t1.val[1]),
                             cv::Scalar::all(255.0f));
                    cv::line(out,
                             cv::Point2f(t0.val[0]+5.0f,t0.val[1]),
                             cv::Point2f(t1.val[0]+5.0f,t1.val[1]),
                             cv::Scalar::all(255.0f));
                    cv::line(out,
                             cv::Point2f(t1.val[0]-5.0f,t1.val[1]),
                             cv::Point2f(t1.val[0]+5.0f,t1.val[1]),
                             cv::Scalar::all(255.0f));
                }

            }

            template<typename _t>
            void printProperty(BirdViewImage::Mat& out, Tracklet* tracklet, std::string s, std::string l, cv::Point point) {
                std::stringstream ss;
                if(!tracklet->properties()->template has<_t>(l))
                    return;

                ss << s << ": " << MAX(((int)(tracklet->properties()->template get<_t>(l)*100))/100.0f,0.0f);
                cv::putText(out,ss.str(),point,cv::FONT_HERSHEY_SIMPLEX,0.5,cv::Scalar::all(255.0f));
            }

            void drawTracklet(BirdViewImage::Mat& out, Tracklet* tracklet) {
                cv::Vec4f trackletPoints[9];
                cv::Matx44f TR;
                cv::Vec4f maxTrackletPoint;
                float sgn;

                this->objectPoints(trackletPoints,tracklet->w(),tracklet->l(),tracklet->TR());
                this->drawObjectPoints(out,trackletPoints,cv::Scalar::all(255.0f));


                // find max y-point
                maxTrackletPoint = trackletPoints[0];
                for(int i = 0; i < 9; i++)
                    if(trackletPoints[i].val[1] > maxTrackletPoint.val[1])
                        maxTrackletPoint = trackletPoints[i];
                sgn = maxTrackletPoint.val[0] < out.cols/2.0f ? -1 : 1;

                cv::line(out,
                         cv::Point2f(maxTrackletPoint.val[0]+sgn*5,maxTrackletPoint.val[1]+5),
                         cv::Point2f(maxTrackletPoint.val[0]+sgn*20,maxTrackletPoint.val[1]+20),
                         cv::Scalar::all(255.0f));
                cv::line(out,
                         cv::Point2f(maxTrackletPoint.val[0]+sgn*20,maxTrackletPoint.val[1]+20),
                         cv::Point2f(maxTrackletPoint.val[0]+sgn*80,maxTrackletPoint.val[1]+20),
                         cv::Scalar::all(255.0f));


                cv::putText(out,"l: " + tracklet->label(),cv::Point2f(maxTrackletPoint.val[0]+sgn*52-30,maxTrackletPoint.val[1]+32),cv::FONT_HERSHEY_SIMPLEX,0.5,cv::Scalar::all(255.0f));
                this->printProperty<float>(out,tracklet,"s","saliency",cv::Point2f(maxTrackletPoint.val[0]+sgn*52-30,maxTrackletPoint.val[1]+52));
                this->printProperty<float>(out,tracklet,"r","recall",cv::Point2f(maxTrackletPoint.val[0]+sgn*52-30,maxTrackletPoint.val[1]+72));
            }

            void drawTracklets(BirdViewImage::Mat& out) {
                TrackletList* t;

                t = this->template input<2>()->value();

                for(int i = 0 ; i < t->size(); i++)
                    this->drawTracklet(out,(*t)[i]);
            }

            void drawVelodyne(BirdViewImage::Mat& out) {
                VelodyneReader::Matrix* mat;
                cv::Vec4f* v, p;
                cv::Point c;

                mat = this->template input<1>()->value();
                v = (cv::Vec4f*)mat->data();

                for(int i = 0; i < mat->height(); i++) {
                    p = this->m_TR*cv::Vec4f(v[i].val[0],v[i].val[1],v[i].val[2],1.0f);
                    c = cv::Point2f(p.val[0],p.val[1]);
                    if(!cv::Rect(0,0,out.cols,out.rows).contains(c))
                        continue;
                    out.template at<cv::Vec3b>(c) = cv::Vec3b(128,128,128);
                }
            }

            cv::Scalar jet(float v, float s = 1.0f) {
                cv::Scalar c = cv::Scalar::all(1.0f); // white

                if (v < 0.25) { c.val[2] = 0; c.val[1] = 4*v;
                } else if (v < 0.5) { c.val[2] = 0; c.val[0] = 1 + 4 * (0.25 - v);
                } else if (v < 0.75) {  c.val[2] = 4 * (v - 0.5); c.val[0] = 0;
                } else { c.val[1] = 1 + 4 * (0.75 - v); c.val[0] = 0; }

                c.val[0]*=255.0f*s;
                c.val[1]*=255.0f*s;
                c.val[2]*=255.0f*s;

                return(c);
            }


#define DEG2RAD(deg) ((deg)/180.0f)*float(M_PI)

            void drawFOV(BirdViewImage::Mat& out, cv::Vec2f angle, utils::PolarHeatmapImage *fov, utils::PolarHeatmapImage *depth, float maxDepth) {
                cv::Vec2f angles[4] = {
                        {DEG2RAD(angle.val[0]-0.5f),DEG2RAD(angle.val[1]-0.5f)},
                        {DEG2RAD(angle.val[0]+0.5f),DEG2RAD(angle.val[1]-0.5f)},
                        {DEG2RAD(angle.val[0]+0.5f),DEG2RAD(angle.val[1]+0.5f)},
                        {DEG2RAD(angle.val[0]-0.5f),DEG2RAD(angle.val[1]+0.5f)}};
                float f, d;
                cv::Vec4f points[4], center;
                cv::Point c[4];

                f = fov->template at<float>(cv::Point((int)angle.val[0],(int)angle.val[1]));
                if(f < FLT_EPSILON)
                    return;

                d = depth->template at<float>(cv::Point((int)angle.val[0],(int)angle.val[1]));
                if(d < FLT_EPSILON)
                    d = maxDepth;

                for(int i = 0; i < 4; i++) {
                    points[i].val[0] = d * sinf(angles[i].val[1]) * cosf(angles[i].val[0]);
                    points[i].val[1] = d * sinf(angles[i].val[1]) * sinf(angles[i].val[0]);
                    points[i].val[2] = d * cosf(angles[i].val[1]);
                    points[i].val[3] = 1.0f;
                    points[i] = this->m_TR * points[i];
                    c[i].x = (int)points[i].val[0];
                    c[i].y = (int)points[i].val[1];
                }

                cv::fillConvexPoly(out,c,4,this->jet(f));

                center = this->m_TR * cv::Vec4f(0.0f,0.0f,0.0f,1.0f);
                for(int i = 0; i < 4; i++)
                    cv::line(out,cv::Point((int)center.val[0],(int)center.val[1]),c[i],this->jet(f,0.25f));
            }

            void drawFOV(BirdViewImage::Mat& out) {
                saliency_sandbox::utils::PolarHeatmapImage *fov, *dep, fovn;
                double maxR;

                fov = this->template input<3>()->value();
                dep = this->template input<4>()->value();

                cv::minMaxLoc(dep->mat(), nullptr,&maxR);
                cv::normalize(fov->mat(),fovn.mat(),0,1,cv::NORM_MINMAX);

                for(int i = 0; i < fov->width(); i++)
                    for(int j = 0; j < fov->height(); j++)
                        this->drawFOV(out,cv::Vec2f(i,j),&fovn,dep,float(maxR));
            }

            void calc() override {
                BirdViewImage::Mat out;

                this->m_out.clear();
                out = this->m_out.mat();

                this->drawFOV(out);
                this->drawVelodyne(out);
                this->drawTracklets(out);
                this->drawEgoTrajectory(out);
            }

            void reset() override {

            }
        };

    }
}

#endif //DRAWBIRDVIEW_H
