//
// Created by geislerd on 13.02.17.
//

#include <kitti/KittiReader.h>

namespace saliency_sandbox {
    namespace kitti {
        KittiReader::KittiReader(boost::filesystem::path path) : m_path(path), m_velodyne(path), m_image_left_gray(path), m_image_right_gray(path), m_image_left_rgb(path), m_image_right_rgb(path), m_oxts(path), m_tracklet(path), m_calibration(path) {
            this->template output<0>()->name("Velodyne");
            this->template output<0>()->connect(this->m_velodyne.template output<0>());

            this->template output<1>()->name("Camera 0");
            this->template output<1>()->connect(this->m_image_left_gray.template output<0>());

            this->template output<2>()->name("Camera 1");
            this->template output<2>()->connect(this->m_image_right_gray.template output<0>());

            this->template output<3>()->name("Camera 2");
            this->template output<3>()->connect(this->m_image_left_rgb.template output<0>());

            this->template output<4>()->name("Camera 3");
            this->template output<4>()->connect(this->m_image_right_rgb.template output<0>());

            this->template output<5>()->name("OXTS");
            this->template output<5>()->connect(this->m_oxts.template output<0>());

            this->template output<6>()->name("Tracklet");
            this->template output<6>()->connect(this->m_tracklet.template output<0>());

            this->template output<7>()->name("Calibration");
            this->template output<7>()->connect(this->m_calibration.template output<0>());
        }

        void KittiReader::calc() {
            this->m_velodyne.process(this->time());
            this->template output<0>()->value(this->m_velodyne.template output<0>()->value());

            this->m_image_left_gray.process(this->time());
            this->template output<1>()->value(this->m_image_left_gray.template output<0>()->value());

            this->m_image_right_gray.process(this->time());
            this->template output<2>()->value(this->m_image_right_gray.template output<0>()->value());

            this->m_image_left_rgb.process(this->time());
            this->template output<3>()->value(this->m_image_left_rgb.template output<0>()->value());

            this->m_image_right_rgb.process(this->time());
            this->template output<4>()->value(this->m_image_right_rgb.template output<0>()->value());

            this->m_oxts.process(this->time());
            this->template output<5>()->value(this->m_oxts.template output<0>()->value());

            this->m_tracklet.process(this->time());
            this->template output<6>()->connect(this->m_tracklet.template output<0>());

            this->m_calibration.process(this->time());
            this->template output<7>()->connect(this->m_calibration.template output<0>());

            this->eof(this->m_velodyne.eof() | this->m_image_left_gray.eof() | this->m_image_right_gray.eof() | this->m_image_left_rgb.eof() | this->m_image_right_rgb.eof() | this->m_oxts.eof() | this->m_tracklet.eof() | this->m_calibration.eof());
        }

        void KittiReader::reset() {

        }
    }
}