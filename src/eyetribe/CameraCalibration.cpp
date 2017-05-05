//
// Created by geislerd on 14.04.17.
//

#include <eyetribe/CameraCalibration.h>

namespace saliency_sandbox {
    namespace eyetribe {
        template <uint32_t _format>
        CameraCalibration<_format>::CameraCalibration() {
            this->template input<0>()->name("image");

            this->template output<0>()->name("matrix");
            this->template output<0>()->value(&this->m_camera_matrix);
            this->template output<1>()->name("distortion");
            this->template output<1>()->value(&this->m_camera_distortion);
            this->template output<2>()->name("valid");
            this->template output<2>()->value(&this->m_valid);

            this->reset();
        }


        template <uint32_t _format>
        void CameraCalibration<_format>::calc() {
            const CameraCornersList& camera_corners = *this->template input<0>()->value();
            const ObjectCornersList& object_corners = *this->template input<1>()->value();
            const bool& valid = *this->template input<2>()->value();

            this->m_valid |= this->properties()->template get<bool>("fix",true);

            if(!valid)
                return;

            if(this->m_valid)
                return;

            this->m_error = (float)cv::calibrateCamera(
                    object_corners,
                    camera_corners,
                    cv::Size(Format<_format>::WIDTH,Format<_format>::HEIGHT),
                    this->m_camera_matrix,
                    this->m_camera_distortion,
                    cv::noArray(),
                    cv::noArray(),
                    cv::CALIB_USE_INTRINSIC_GUESS|
                    cv::CALIB_FIX_K1|
                    cv::CALIB_FIX_K2|
                    cv::CALIB_FIX_K3|
                    cv::CALIB_FIX_K4|
                    cv::CALIB_FIX_K5|
                    cv::CALIB_FIX_K6|
                    cv::CALIB_ZERO_TANGENT_DIST|
                    cv::CALIB_FIX_PRINCIPAL_POINT|
                    cv::CALIB_FIX_ASPECT_RATIO,
                    cv::TermCriteria(cv::TermCriteria::COUNT+cv::TermCriteria::EPS, 30, FLT_EPSILON));

            std::cout << "Camera Calibration: " << std::endl;
            std::cout << "M: " << this->m_camera_matrix << std::endl;
            std::cout << "E: " << this->m_error << std::endl;

            this->m_valid = true;
        }


        template <uint32_t _format>
        void CameraCalibration<_format>::reset() {
            const float sensor_width = 4.80f;
            const float sensor_height = 3.60f;
            const float focal_length = 4.55f;

            const float pixels_per_mm_H = Format<4>::WIDTH/sensor_width;
            const float pixels_per_mm_V = Format<4>::HEIGHT/sensor_height;

            const float focal_length_H = focal_length * pixels_per_mm_H;
            const float focal_length_V = focal_length * pixels_per_mm_V;

            this->m_camera_matrix.template at<float>(0) = focal_length_H;
            this->m_camera_matrix.template at<float>(1) = 0.0f;
            this->m_camera_matrix.template at<float>(2) = Format<_format>::WIDTH/2.0f;//480;//Format<_format>::WIDTH/2.0f;
            this->m_camera_matrix.template at<float>(3) = 0.0f;
            this->m_camera_matrix.template at<float>(4) = focal_length_V;
            this->m_camera_matrix.template at<float>(5) = Format<_format>::HEIGHT/2.0f;//960;//Format<_format>::HEIGHT/2.0f;
            this->m_camera_matrix.template at<float>(6) = 0.0f;
            this->m_camera_matrix.template at<float>(7) = 0.0f;
            this->m_camera_matrix.template at<float>(8) = 1.0f;

            this->m_camera_distortion.clear();

            this->m_valid = false;
            this->m_error = 0.0f;

            std::cout << "Camera Calibration: " << std::endl;
            std::cout << "M: " << this->m_camera_matrix << std::endl;
            std::cout << "E: " << this->m_error << std::endl;
        }

        template class CameraCalibration<0>;
        template class CameraCalibration<1>;
        template class CameraCalibration<2>;
        template class CameraCalibration<3>;
        template class CameraCalibration<4>;
        template class CameraCalibration<5>;
        template class CameraCalibration<6>;
        template class CameraCalibration<7>;
    }
}