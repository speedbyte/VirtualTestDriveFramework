//
// Created by geislerd on 12.04.17.
//

#ifndef EYETRIBE_STEREOCALIBRATION_H
#define EYETRIBE_STEREOCALIBRATION_H

#include <eyetribe/CameraCalibration.h>

namespace saliency_sandbox {
    namespace eyetribe {

        typedef saliency_sandbox::utils::_Matrix<3,3,float> CameraCameraRotation;
        typedef saliency_sandbox::utils::_Matrix<1,3,float> CameraCameraTranslation;
        typedef saliency_sandbox::utils::_Matrix<3,3,float> EssentialMatrix;
        typedef saliency_sandbox::utils::_Matrix<3,3,float> FundamentalMatrix;

        template<uint32_t _format>
        class StereoCalibration : public saliency_sandbox::core::Node::
            template Input<
                CameraCorners,             // calibration pattern points in camera 0
                CameraCorners,             // calibration pattern points in camera 1
                CameraMatrix,              // camera 0 matrix
                CameraDistortion,          // camera 0 distortion
                CameraMatrix,              // camera 1 matrix
                CameraDistortion,          // camera 1 distortion
                bool,                      // camera 0 is calibrated
                bool                       // camera 1 is calibrated
            >::template Output<
                CameraCameraRotation,      // rotation matrix from camera 0 to camera 1
                CameraCameraTranslation,   // translation from camera 0 to camera 1
                EssentialMatrix,           // essential matrix
                FundamentalMatrix,         // fundamental matrix
                bool                       // calibration was successful
            > {
        private:
            ObjectCorners m_object_corners;
            ObjectCornersList m_object_corners_list;
            CameraCornersList m_camera_corners_list[2];

            CameraCameraRotation m_R;
            CameraCameraTranslation m_T;
            EssentialMatrix m_E;
            FundamentalMatrix m_F;

            std::chrono::high_resolution_clock::time_point m_time;

            bool m_valid;
            float m_error;

            bool collectPoints();

        public:
            StereoCalibration();
            void calc() override;
            void reset() override;

        };
    }
}

#endif //EYETRIBE_STEREOCALIBRATION_H
