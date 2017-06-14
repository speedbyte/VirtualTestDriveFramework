//
// Created by geislerd on 17.03.17.
//

#ifndef OXTSPOSE_H
#define OXTSPOSE_H

#include <core/Node.h>
#include <kitti/OXTSReader.h>
#include <kitti/CalibrationReader.h>
#include <flow/Trajectory.h>

namespace saliency_sandbox {
    namespace kitti {

        class OXTSPose : public saliency_sandbox::core::Node::
        template Input<OXTSRow,Calibration>::
        template Output<saliency_sandbox::flow::Pose> {
        private:
            saliency_sandbox::flow::Pose m_pose;
            cv::Matx44f m_imu2velo, m_velo2imu;
            bool m_calib;

        public:
            OXTSPose();

            void calc() override;

            void reset() override;
        };
    }
}
#endif //OXTSPOSE_H
