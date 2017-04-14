//
// Created by geislerd on 13.04.17.
//

#ifndef EYETRIBE_CAMERACALIBRATION_H
#define EYETRIBE_CAMERACALIBRATION_H

#include <eyetribe/VideoReader.h>

#define EYETRIBE_CHECKERBOARD_ROWS 4
#define EYETRIBE_CHECKERBOARD_COLS 4
#define EYETRIBE_CHECKERBOARD_NUM (EYETRIBE_CHECKERBOARD_ROWS*EYETRIBE_CHECKERBOARD_COLS)
#define EYETRIBE_CHECKERBOARD_SIZE cv::Size(EYETRIBE_CHECKERBOARD_ROWS,EYETRIBE_CHECKERBOARD_COLS)
#define EYETRIBE_CHECKERBOARD_DISTANCE float(1.0f/(float(EYETRIBE_CHECKERBOARD_ROWS)-1.0f)*(40.0f/3.0f))
#define EYETRIBE_CHECKERBOARD_OBJECT_COORD(x,y) cv::Vec3f(EYETRIBE_CHECKERBOARD_DISTANCE*float(x),EYETRIBE_CHECKERBOARD_DISTANCE*float(y),0.0f)

namespace saliency_sandbox {
    namespace eyetribe {

        typedef saliency_sandbox::utils::_Matrix<3,3,float> CameraMatrix;
        typedef saliency_sandbox::utils::_Matrix<5,1,float> CameraDistortion;
        typedef std::vector<cv::Vec2f> CameraCorners;
        typedef std::vector<cv::Vec3f> ObjectCorners;
        typedef std::vector<CameraCorners> CameraCornersList;
        typedef std::vector<ObjectCorners> ObjectCornersList;

        template <uint32_t _format>
        class CameraCalibration : public saliency_sandbox::core::Node::
        template Input<typename VideoReader<_format>::Image>::
        template Output<CameraMatrix,CameraDistortion,bool,float,CameraCorners,bool> {
        private:
            CameraMatrix m_camera_matrix;
            CameraDistortion m_camera_distortion;
            CameraCorners m_camera_corners;
            CameraCorners m_camera_corners_mean;
            ObjectCorners m_object_corners;
            ObjectCornersList m_object_corners_list;
            CameraCornersList m_camera_corners_list;

            bool m_has_corners;
            bool m_is_calibrated;
            float m_coverage;
            float m_error;

            float removePoints();

            bool findCorners(cv::Mat1b in);

        public:
            CameraCalibration();

            void calc() override;

            void reset() override;
        };

    }
}

#endif //EYETRIBE_CAMERACALIBRATION_H
