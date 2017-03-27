//
// Created by geislerd on 16.03.17.
//

#ifndef EGOMOTION_H
#define EGOMOTION_H

#include <flow/Flow.h>
#include <flow/Trajectory.h>
#include <kitti/VelodyneReader.h>
#include <kitti/OXTSReader.h>
#include <kitti/CalibrationReader.h>
#include <kitti/ImageReader.h>
#include <kitti/OXTSPose.h>

namespace saliency_sandbox {
    namespace kitti {

        template<Camera _cam>
        class EgoMotion : public saliency_sandbox::flow::_Flow<ImageReader<_cam>::Image::WIDTH, ImageReader<_cam>::Image::HEIGHT,VelodyneReader::Matrix,saliency_sandbox::flow::Pose,Calibration> {
        public:
            static const size_t WIDTH = ImageReader<_cam>::Image::WIDTH;
            static const size_t HEIGHT = ImageReader<_cam>::Image::HEIGHT;

            EgoMotion();

            Calibration* calibration();

            cv::Matx44f oxts();

            VelodyneReader::Matrix* velodyne();

            void calc() override;

            void reset() override;

            void denseInterp(std::vector<cv::Point2f>& points);
        };

        typedef EgoMotion<LEFT_GRAY> LeftGrayEgoMotion;
        typedef EgoMotion<RIGHT_GRAY> RightGrayEgoMotion;
        typedef EgoMotion<LEFT_RGB> LeftRGBEgoMotion;
        typedef EgoMotion<RIGHT_RGB> RightRGBEgoMotion;
    }
}

#endif //EGOMOTION_H
