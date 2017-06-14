//
// Created by geislerd on 13.04.17.
//

#ifndef EYETRIBE_DRAWCALIBRATIONCORNERS_H
#define EYETRIBE_DRAWCALIBRATIONCORNERS_H

#include <eyetribe/CameraCalibration.h>

namespace saliency_sandbox {
    namespace eyetribe {
        template <uint32_t _format>
        class DrawCalibrationCorners : public saliency_sandbox::core::Node::
        template Input<typename VideoReader<_format>::Image,CameraCorners,bool>::
        template Output<typename VideoReader<_format>::Image> {
        private:
            typename VideoReader<_format>::Image m_image;
        public:
            DrawCalibrationCorners();
            void calc() override;
            void reset() override;
        };
    }
}
#endif //EYETRIBE_DRAWCALIBRATIONCORNERS_H
