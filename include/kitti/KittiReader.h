//
// Created by geislerd on 09.02.17.
//

#ifndef KITTIREADER_H
#define KITTIREADER_H

#include <kitti/VelodyneReader.h>
#include <kitti/ImageReader.h>
#include <kitti/OXTSReader.h>
#include <kitti/tracklets.h>
#include <kitti/TrackletReader.h>
#include <kitti/CalibrationReader.h>

namespace saliency_sandbox {
    namespace kitti {

        class KittiReader : public core::Node::template Input<>::template Output<
                LeftGrayImageReader::Image,
                RightGrayImageReader::Image,
                LeftRGBImageReader::Image,
                RightRGBImageReader::Image,
                VelodyneReader::Matrix,
                OXTSRow,
                TrackletList,
                Calibration> {
        private:
            boost::filesystem::path m_path;
            LeftGrayImageReader m_image_left_gray;
            RightGrayImageReader m_image_right_gray;
            LeftRGBImageReader m_image_left_rgb;
            RightRGBImageReader m_image_right_rgb;
            VelodyneReader m_velodyne;
            OXTSReader m_oxts;
            TrackletReader m_tracklet;
            CalibrationReader m_calibration;

        public:
            KittiReader(boost::filesystem::path path);

            void calc() override;

            void reset();
        };
    }
}

#endif //KITTIREADER_H
