//
// Created by geislerd on 24.03.17.
//

#include <kitti/OXTSPose.h>


namespace saliency_sandbox {
    namespace kitti {
        OXTSPose::OXTSPose() : m_calib(false) {
            this->template input<0>()->name("oxts");
            this->template input<1>()->name("calibration");

            this->template output<0>()->name("pose");
            this->template output<0>()->value(&this->m_pose);
        }

        void OXTSPose::calc() {
            OXTSRow* oxts;
            Calibration* calib;
            cv::Vec3f t, r;
            cv::Matx33f R;
            cv::Matx44f P;
            cv::Mat1f pose;

            oxts = this->template input<0>()->value();
            calib = this->template input<1>()->value();

            if(!this->m_calib) { // read calibration
                this->m_imu2velo = calib->imuToVeloTR();
                this->m_velo2imu = this->m_imu2velo.inv(cv::DECOMP_LU);
                this->m_calib = true;
            }

            // translation vector
            t = cv::Vec3f(
                    oxts->vf(), // forward (x) velocity
                    oxts->vl(), // leftward (y) velocity
                    oxts->vu()  // upward (z) velocity
            ) / 10.0f;          // per frame

            // rotation matrix (OXTS RT3000 user manual, page 71/92)
            r = cv::Vec3f(
                    oxts->wf(), // rotation rate around forward (x) axis
                    oxts->wl(), // rotation rate around leftward (y) axis
                    oxts->wu()  // rotation rate around upward (z) axis
            ) / 10.0f;          // per frame
            R = cv::Matx33f::eye();
            R = R * cv::Matx33f(1.0f,0.0f,0.0f,0.0f,cosf(r.val[0]),-sinf(r.val[0]),0.0f,sinf(r.val[0]),cosf(r.val[0])); // rotate around forward (x) axis
            R = R * cv::Matx33f(cosf(r.val[1]),0.0f,sinf(r.val[1]),0.0f,1.0f,0.0f,-sinf(r.val[1]),0.0f,cosf(r.val[1])); // rotate around leftward (y) axis
            R = R * cv::Matx33f(cosf(r.val[2]),-sinf(r.val[2]),0.0f,sinf(r.val[2]),cosf(r.val[2]),0.0f,0.0f,0.0f,1.0f); // rotate around upward (z) axis

            // assamble transformation matrix
            P = cv::Matx44f(
                    R.val[0],R.val[1],R.val[2],t.val[0],
                    R.val[3],R.val[4],R.val[5],t.val[1],
                    R.val[6],R.val[7],R.val[8],t.val[2],
                    0.0f,0.0f,0.0f,1.0f);

            P = (this->m_velo2imu * P) * this->m_imu2velo;

            // add pose
            pose = cv::Mat1f(4,4,P.val);
            this->m_pose.mat(pose);
        }

        void OXTSPose::reset() {

        }
    }
}