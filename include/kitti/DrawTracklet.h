//
// Created by geislerd on 16.03.17.
//

#ifndef DRAWTRACKLET_H
#define DRAWTRACKLET_H

#include <kitti/ImageReader.h>

namespace saliency_sandbox {
    namespace kitti {

        template<Camera _camera>
        class DrawTracklet : public saliency_sandbox::core::Node::template Input<
                typename ImageReader<_camera>::Image,
                TrackletList,
                Calibration>::template Output<
                saliency_sandbox::utils::_RGBImage<1242,375>> {
        public:
            typedef typename ImageReader<_camera>::Image input_image_t;
        private:
            saliency_sandbox::utils::_RGBImage<1242,375> m_data;

            void convert(input_image_t& img);

            bool inImage(cv::Point* points, size_t num);

            void drawTracklet(Tracklet& tracklet, Calibration& calibration);

            void drawTracklets(TrackletList& tracklets, Calibration& calibration);

        public:

            DrawTracklet();

            void calc() override;

            void reset() override;
        };

        typedef DrawTracklet<LEFT_GRAY> LeftGrayDrawTracklet;
        typedef DrawTracklet<RIGHT_GRAY> RightGrayDrawTracklet;
        typedef DrawTracklet<LEFT_RGB> LeftRGBDrawTracklet;
        typedef DrawTracklet<RIGHT_RGB> RightRGBDrawTracklet;
    }
}
#endif //DRAWTRACKLET_H
