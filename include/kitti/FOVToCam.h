//
// Created by geislerd on 23.03.17.
//

#ifndef FOVTOCAM_H
#define FOVTOCAM_H

#include <kitti/ImageReader.h>
#include <utils/Polar.h>

namespace saliency_sandbox {
    namespace kitti {

        template <Camera _cam>
        class FOVToCam : public saliency_sandbox::core::Node::
        template Input<saliency_sandbox::utils::PolarHeatmapImage,saliency_sandbox::utils::PolarHeatmapImage,saliency_sandbox::kitti::Calibration>::
        template Output<saliency_sandbox::utils::_HeatmapImage<saliency_sandbox::kitti::ImageReader<_cam>::Image::WIDTH,saliency_sandbox::kitti::ImageReader<_cam>::Image::HEIGHT>> {
        public:
            typedef saliency_sandbox::utils::_HeatmapImage<saliency_sandbox::kitti::ImageReader<_cam>::Image::WIDTH,saliency_sandbox::kitti::ImageReader<_cam>::Image::HEIGHT> Output;
        private:
            Output m_output;

        public:
            FOVToCam();

            void poly(cv::Vec3f* p, int inclination, int azimuth, float r);

            void calc() override;

            void reset() override;
        };

    }
}

#endif //FOVTOCAM_H
