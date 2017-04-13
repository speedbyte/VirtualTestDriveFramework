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
            DrawCalibrationCorners() {
                this->template input<0>()->name("image");
                this->template input<1>()->name("corners");
                this->template input<2>()->name("valid");

                this->template output<0>()->name("image");
                this->template output<0>()->value(&this->m_image);
            }

            void calc() override {
                cv::Mat1b in;
                cv::Mat2f corners;
                bool valid;

                in = this->template input<0>()->value()->mat();
                corners = cv::Mat2f(16,1,(cv::Vec2f*)this->template input<1>()->value()->data());
                valid = this->template input<2>()->value()[0];

                this->m_image.mat(in);

                if(valid)
                    cv::drawChessboardCorners(this->m_image.mat(),cv::Size(4,4),corners,valid);
            }

            void reset() override {

            }
        };
    }
}
#endif //EYETRIBE_DRAWCALIBRATIONCORNERS_H
