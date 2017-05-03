//
// Created by geislerd on 02.05.17.
//

#ifndef EYETRIBE_PUPILDETECTION_H
#define EYETRIBE_PUPILDETECTION_H

#include <eyetribe/EyeballEstimation.h>
#include <gaze/Gaze.h>
#include "EyeballEstimation.h"
#include "StereoGlintMatcher.h"

namespace saliency_sandbox {
    namespace eyetribe {

        template<uint32_t _format>
        class PupilDetection : public saliency_sandbox::core::Node::
        template Input<
                typename Format<_format>::Image,
                typename Format<_format>::Image,
                RectificationProjection,
                RectificationProjection,
                Eyeball
        >::template Output<
                gaze::Gaze,
                float
        > {
        private:
            gaze::Gaze m_gaze;
            float m_confidence;

        public:
            PupilDetection() : m_gaze(0.0f,0.0f,0.0f,0.0f,0.0f), m_confidence(0.0f) {
                this->template input<0>()->name("left image");
                this->template input<1>()->name("right image");
                this->template input<2>()->name("left projection");
                this->template input<3>()->name("right projection");
                this->template input<4>()->name("eyeball");

                this->template output<0>()->name("gaze");
                this->template output<0>()->value(&this->m_gaze);
                this->template output<1>()->name("confidence");
                this->template output<1>()->value(&this->m_confidence);

                this->reset();
            }

            void getEyeballBoundingBox(const Eyeball &eb, cv::Point3f bb[2]) {
                bb[0].x = eb.center.x - eb.radius;
                bb[0].y = eb.center.y - eb.radius;
                bb[0].z = eb.center.z - eb.radius;

                bb[1].x = eb.center.x + eb.radius;
                bb[1].y = eb.center.y + eb.radius;
                bb[1].z = eb.center.z + eb.radius;
            }

            void getEyeballBoundingRect(const cv::Point3f bb[2], const RectificationProjection &p, cv::Rect& rect) {
                cv::Matx34f P(p);
                cv::Vec4f v4[2];
                cv::Vec3f v3[2];
                cv::Vec2f v2[2];

                for(int i = 0; i < 2; i++) {
                    v4[i] = cv::Vec4f(bb[i].x,bb[i].y,bb[i].z,1.0f);
                    v3[i] = P * v4[i];
                    v2[i] = cv::Vec2f(v3[i].val[0]/v3[i].val[2],v3[i].val[1]/v3[i].val[2]);
                }

                rect.x = int(floor(MIN(v2[0].val[0],v2[1].val[0])));
                rect.y = int(floor(MIN(v2[0].val[1],v2[1].val[1])));
                rect.width = int(ceil(MAX(v2[0].val[0],v2[1].val[0])))-rect.x;
                rect.height = int(ceil(MAX(v2[0].val[1],v2[1].val[1])))-rect.y;
            }

            void removeGlint(const cv::Mat1b &in, cv::Mat1b& out) {
                cv::Mat1b t, tg, td, inn;
                cv::Point p;
                cv::Scalar m;

                cv::normalize(in,inn,0,255,cv::NORM_MINMAX);
                cv::threshold(inn, t, 64, 255, cv::THRESH_BINARY);

                out = in;

                for(int i = 0; true; i++) {
                    p = cv::Point(-1,-1);
                    cv::minMaxLoc(out, nullptr, nullptr, nullptr, &p, t);
                    if(p.x < 0 || p.y < 0)
                        return;

                    tg = cv::Mat1b::zeros(t.rows+2,t.cols+2);
                    cv::floodFill(t,tg,p,255, nullptr,cv::Scalar(),cv::Scalar(),4 + (255 << 8) + cv::FLOODFILL_MASK_ONLY);
                    tg = tg(cv::Rect(1,1,t.cols,t.rows));
                    cv::dilate(tg,td,cv::Mat1b::ones(3,3));
                    cv::bitwise_xor(td,tg,td);
                    out.setTo(cv::mean(out,td),tg);
                    t.setTo(cv::Scalar::all(0),tg);
                }


            }

            cv::Point detectPupil(const cv::Mat1b& img) {
                cv::Mat1b imgm,imgn,imgt;
                cv::Mat1f imgf;
                cv::Point p;

                this->removeGlint(img,imgm);

                cv::subtract(cv::Scalar::all(255),imgm,imgm);
                cv::normalize(imgm,imgm,0,255,cv::NORM_MINMAX);

                for(int i = 0; i < 4; i++) {
                    cv::blur(imgm,imgm,cv::Size(5,5));
                    cv::subtract(imgm,cv::Scalar::all(128),imgm);
                    cv::normalize(imgm,imgm,0,255,cv::NORM_MINMAX);
                }

                cv::threshold(imgm, imgt, 128, 255, cv::THRESH_BINARY);

                p = cv::Point(-1,-1);
                cv::minMaxLoc(imgm, nullptr, nullptr, nullptr,&p,imgt);

                return p;
            }

            void triangulatePoints(cv::Point2f p[2], cv::Point3f& pupil, const RectificationProjection& lp, const RectificationProjection& rp) {
                cv::Mat1f left, right, world;
                float scale;

                left = cv::Mat1f(2,1);
                right = cv::Mat1f(2,1);
                world = cv::Mat1f(4,1);

                left.template at<float>(0,0) = p[0].x;
                left.template at<float>(1,0) = p[0].y;
                right.template at<float>(0,0) = p[1].x;
                right.template at<float>(1,0) = p[1].y;

                cv::triangulatePoints(lp,rp,left,right,world);

                scale = world.template at<float>(3,0);
                pupil.x = world.template at<float>(0,0) / scale;
                pupil.y = world.template at<float>(1,0) / scale;
                pupil.z = world.template at<float>(2,0) / scale;
            }

            void calc() override {
                const cv::Mat1b& limg = *this->template input<0>()->value();
                const cv::Mat1b& rimg = *this->template input<1>()->value();
                const RectificationProjection& lp = *this->template input<2>()->value();
                const RectificationProjection& rp = *this->template input<3>()->value();
                const Eyeball& eb = *this->template input<4>()->value();

                cv::Point3f ebbb[2], pupil, dir;
                cv::Rect ebbr[2];
                cv::Mat1b eye[2], eyers[2];
                cv::Point2f p[2];
                float n;

                this->m_confidence = 0;

                this->

                return;

                if(eb.glint.confidence < FLT_EPSILON)
                    return;

                this->getEyeballBoundingBox(eb,ebbb);
                this->getEyeballBoundingRect(ebbb,lp,ebbr[0]);
                this->getEyeballBoundingRect(ebbb,rp,ebbr[1]);

                ebbr[0] &= cv::Rect(0,0,limg.cols,limg.rows);
                ebbr[1] &= cv::Rect(0,0,rimg.cols,rimg.rows);

                if (ebbr[0].area() <= 0)
                    return;
                if (ebbr[1].area() <= 0)
                    return;

                eye[0] = limg(ebbr[0]);
                eye[1] = rimg(ebbr[1]);

                for(int i = 0; i < 2; i++) {
                    cv::resize(eye[i], eyers[i], cv::Size(100, 100));
                    p[i] = this->detectPupil(eyers[i]);

                    if(p[i].x < 0.0f || p[i].y < 0.0f)
                        return;

                    p[i].x *= float(eye[i].cols) / float(eyers[i].cols);
                    p[i].y *= float(eye[i].rows) / float(eyers[i].rows);
                    p[i].x += ebbr[i].x;
                    p[i].y += ebbr[i].y;
                }

                this->triangulatePoints(p,pupil,lp,rp);

                dir = pupil-eb.center;
                n = float(cv::norm(dir));
                std::cout << "n: " << n << std::endl;
                dir.x /= n;
                dir.y /= n;
                dir.z /= n;

                this->m_gaze = gaze::Gaze(eb.center,dir);
            }

            void reset() override {}
        };
    }
}

#endif //EYETRIBE_PUPILDETECTION_H
