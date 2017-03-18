//
// Created by geislerd on 17.03.17.
//

#ifndef OXTSPOSE_H
#define OXTSPOSE_H

#include <core/Node.h>
#include <utils/Matrix.h>
#include <kitti/OXTSReader.h>
#include <kitti/CalibrationReader.h>
#include <flow/Trajectory.h>

namespace saliency_sandbox {
    namespace kitti {

        class OXTSPose : public saliency_sandbox::core::Node::
        template Input<OXTSRow>::
        template Output<saliency_sandbox::flow::Pose> {
        private:
            saliency_sandbox::flow::Pose m_pose;
            float m_scale;
            cv::Matx44f m_Tr_0_inv;

        public:
            OXTSPose() : m_scale(0.0f) {
                this->template input<0>()->name("oxts");

                this->template output<0>()->name("pose");
                this->template output<0>()->value(&this->m_pose);
            }

            float latToScale(float lat) {
                return cosf(lat * float(M_PI) / 180.0f);
            }

            cv::Vec3f llaToMercator(float lat, float lon, float alt, float scale) {
                cv::Vec3f r;
                float er;

                float cosLat = cosf(lat * float(M_PI) / 180.0f);
                float sinLat = sinf(lat * float(M_PI) / 180.0f);
                float cosLon = cosf(lon * float(M_PI) / 180.0f);
                float sinLon = sinf(lon * float(M_PI) / 180.0f);
scale = alt+6378137;
                //er = 6378137;
                //r.val[0] = this->m_scale * lon * float(M_PI) * er / 180.0f;
                //r.val[1] = this->m_scale * er * logf( tanf((90.0f+lat) * float(M_PI) / 360.0f) );
                //r.val[2] = alt;
                r.val[1] = sinLat * cosLon;
                r.val[0] = sinLat * sinLon;
                r.val[2] = cosLat;

                return r;
            }

            void calc() override {
                OXTSRow* oxts;
                cv::Vec3f t, r;
                cv::Matx33f Rx, Ry, Rz, R, S, T;
                cv::Matx44f P;
                cv::Mat1f pose;

                oxts = this->template input<0>()->value();

                // compute scale from first lat value
                if(this->m_scale < FLT_EPSILON)
                    this->m_scale = OXTSPose::latToScale(oxts->lat());

                // translation vector
                t = cv::Vec3f(oxts->vf(),oxts->vl(),oxts->vu());

                P = cv::Matx44f(
                        0.0f,0.0f,0.0f,t.val[0],
                        0.0f,0.0f,0.0f,t.val[1],
                        0.0f,0.0f,0.0f,t.val[2],
                        0.0f,0.0f,0.0f,1.0f);

                // rotation matrix (OXTS RT3000 user manual, page 71/92)
                r = cv::Vec3f(oxts->wf(),oxts->wl(),oxts->wu());
                Rx = cv::Matx33f(1.0f,0.0f,0.0f,0.0f,cosf(r.val[0]),-sinf(r.val[0]),0.0f,sinf(r.val[0]),cosf(r.val[0]));
                Ry = cv::Matx33f(cosf(r.val[1]),0.0f,sinf(r.val[1]),0.0f,1.0f,0.0f,-sinf(r.val[1]),0.0f,cosf(r.val[1]));
                Rz = cv::Matx33f(cosf(r.val[2]),-sinf(r.val[2]),0.0f,sinf(r.val[2]),cosf(r.val[2]),0.0f,0.0f,0.0f,1.0f);
                R = Rx*Ry*Rz;

                P = cv::Matx44f(
                        R.val[0],R.val[1],R.val[2],0.0f,
                        R.val[3],R.val[4],R.val[5],0.0f,
                        R.val[6],R.val[7],R.val[8],0.0f,
                        0.0f,0.0f,0.0f,1.0f) * P;

                P = cv::Matx44f( //TODO: dont know why but a second rotation is nesseccary
                        R.val[0],R.val[1],R.val[2],0.0f,
                        R.val[3],R.val[4],R.val[5],0.0f,
                        R.val[6],R.val[7],R.val[8],0.0f,
                        0.0f,0.0f,0.0f,1.0f) * P;



                // normalize translation and rotation (start at 0/0/0)
                if(this->m_Tr_0_inv.val[15] < FLT_EPSILON) {
                    this->m_Tr_0_inv = P.inv();
                }

                // add pose
                //P = this->m_Tr_0_inv * P;
                pose = cv::Mat1f(4,4,P.val);
                this->m_pose.mat(pose);
            }

            void reset() override {

            }
        };
    }
}
#endif //OXTSPOSE_H
