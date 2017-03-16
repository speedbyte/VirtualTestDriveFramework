//
// Created by geislerd on 16.03.17.
//

#ifndef EGOMOTION_H
#define EGOMOTION_H

#include <flow/Flow.h>
#include <kitti/VelodyneReader.h>
#include <kitti/OXTSReader.h>
#include <kitti/CalibrationReader.h>
#include "ImageReader.h"

namespace saliency_sandbox {
    namespace kitti {

        template<Camera _cam>
        class _EgoMotion : public saliency_sandbox::flow::_Flow<ImageReader<_cam>::Image::WIDTH, ImageReader<_cam>::Image::HEIGHT,3,VelodyneReader::Matrix,OXTSRow,Calibration> {
        private:
        public:
            _EgoMotion() {
                this->template input<0>()->name("velocity");
                this->template input<1>()->name("depth");
            }

            void calc() override {

            }

            void reset() override {

            }
        };
    }
}

#endif //EGOMOTION_H
