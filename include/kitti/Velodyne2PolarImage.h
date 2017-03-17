//
// Created by geislerd on 16.03.17.
//

#ifndef VELODYNE2POLARIMAGE_H
#define VELODYNE2POLARIMAGE_H

#include <kitti/VelodyneReader.h>

namespace saliency_sandbox {
    namespace kitti {

        typedef saliency_sandbox::utils::_HeatmapImage<360, 180> VelodynePolarImage;

        template<uint32_t _num_points>
        class _Velodyne2PolarImage
                : public saliency_sandbox::core::Node::template Input<typename _VelodyneReader<_num_points>::Matrix>::template Output<VelodynePolarImage, VelodynePolarImage> {
        private:
            VelodynePolarImage m_distance;
            VelodynePolarImage m_reflection;

        public:
            _Velodyne2PolarImage();

            void calc() override;

            void reset() override;
        };

        typedef _Velodyne2PolarImage<VELODYNE_BUFFER_SIZE> Velodyne2PolarImage;
    }
}

#endif //VELODYNE2POLARIMAGE_H
