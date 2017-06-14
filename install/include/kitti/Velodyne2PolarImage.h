//
// Created by geislerd on 16.03.17.
//

#ifndef VELODYNE2POLARIMAGE_H
#define VELODYNE2POLARIMAGE_H

#include <kitti/VelodyneReader.h>
#include <utils/Polar.h>

namespace saliency_sandbox {
    namespace kitti {

        template<uint32_t _num_points>
        class _Velodyne2PolarImage
                : public saliency_sandbox::core::Node::
                template Input<typename _VelodyneReader<_num_points>::Matrix>::
                template Output<saliency_sandbox::utils::PolarHeatmapImage, saliency_sandbox::utils::PolarHeatmapImage> {
        private:
            saliency_sandbox::utils::PolarHeatmapImage m_distance;
            saliency_sandbox::utils::PolarHeatmapImage m_reflection;

        public:
            _Velodyne2PolarImage();

            void calc() override;

            void reset() override;
        };

        typedef _Velodyne2PolarImage<VELODYNE_BUFFER_SIZE> Velodyne2PolarImage;
    }
}

#endif //VELODYNE2POLARIMAGE_H
