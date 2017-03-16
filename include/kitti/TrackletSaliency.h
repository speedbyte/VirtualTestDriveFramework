//
// Created by geislerd on 16.03.17.
//

#ifndef TRACKLETSALIENCY_H
#define TRACKLETSALIENCY_H

#include <kitti/ImageReader.h>

namespace saliency_sandbox {
    namespace kitti {

        template<Camera _camera>
        class TrackletSaliency : public saliency_sandbox::core::Node::template Input<
                TrackletList,
                saliency_sandbox::utils::_HeatmapImage<
                        saliency_sandbox::kitti::ImageReader<_camera>::Image::WIDTH,
                        saliency_sandbox::kitti::ImageReader<_camera>::Image::HEIGHT>,
                Calibration>::template Output<TrackletList> {
        public:
            TrackletSaliency();

            void calc() override;

            void reset() override;
        };

        typedef TrackletSaliency<LEFT_GRAY> LeftGrayTrackletSaliency;
        typedef TrackletSaliency<RIGHT_GRAY> RightGrayTrackletSaliency;
        typedef TrackletSaliency<LEFT_RGB> LeftRGBTrackletSaliency;
        typedef TrackletSaliency<RIGHT_RGB> RightRGBTrackletSaliency;
    }
}

#endif //TRACKLETSALIENCY_H
