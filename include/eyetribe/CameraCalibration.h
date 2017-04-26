//
// Created by geislerd on 13.04.17.
//

#ifndef EYETRIBE_CAMERACALIBRATION_H
#define EYETRIBE_CAMERACALIBRATION_H

#include <eyetribe/CollectCalibrationCorners.h>

#define EYETRIBE_CHECKERBOARD_ROWS 8
#define EYETRIBE_CHECKERBOARD_COLS 8
#define EYETRIBE_CHECKERBOARD_NUM (EYETRIBE_CHECKERBOARD_ROWS*EYETRIBE_CHECKERBOARD_COLS)
#define EYETRIBE_CHECKERBOARD_SIZE cv::Size(EYETRIBE_CHECKERBOARD_ROWS,EYETRIBE_CHECKERBOARD_COLS)
#define EYETRIBE_CHECKERBOARD_DISTANCE 12.7f
#define EYETRIBE_CHECKERBOARD_OBJECT_COORD(x,y) cv::Vec3f(EYETRIBE_CHECKERBOARD_DISTANCE*float(x),EYETRIBE_CHECKERBOARD_DISTANCE*float(y),0.0f)

namespace saliency_sandbox {
    namespace eyetribe {

        typedef saliency_sandbox::utils::_Matrix<3,3,float> CameraMatrix;
        typedef saliency_sandbox::utils::_Matrix<5,1,float> CameraDistortion;

        template <uint32_t _format>
        class CameraCalibration : public saliency_sandbox::core::Node::
        template Input<CameraCornersList,ObjectCornersList,bool>::
        template Output<CameraMatrix,CameraDistortion,bool> {
        private:
            CameraMatrix m_camera_matrix;
            CameraDistortion m_camera_distortion;
            bool m_valid;
            float m_error;

        public:
            CameraCalibration();

            void calc() override;

            void reset() override;
        };

    }
}

#endif //EYETRIBE_CAMERACALIBRATION_H
