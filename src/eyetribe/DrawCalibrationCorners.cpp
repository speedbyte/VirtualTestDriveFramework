//
// Created by geislerd on 14.04.17.
//

#include <eyetribe/DrawCalibrationCorners.h>

namespace saliency_sandbox {
    namespace eyetribe {
        template <uint32_t _format>
        DrawCalibrationCorners<_format>::DrawCalibrationCorners() {
            this->template input<0>()->name("image");
            this->template input<1>()->name("corners");
            this->template input<2>()->name("valid");

            this->template output<0>()->name("image");
            this->template output<0>()->value(&this->m_image);
        }

        template <uint32_t _format>
        void DrawCalibrationCorners<_format>::calc() {
            cv::Mat1b in;
            CameraCorners corners;
            bool valid;

            in = this->template input<0>()->value()->mat();
            corners = *this->template input<1>()->value();
            valid = this->template input<2>()->value()[0];

            this->m_image.mat(in);

            if(valid)
                cv::drawChessboardCorners(this->m_image.mat(),cv::Size(8,8),corners,valid);
        }

        template <uint32_t _format>
        void DrawCalibrationCorners<_format>::reset() {

        }

        template class DrawCalibrationCorners<0>;
        template class DrawCalibrationCorners<1>;
        template class DrawCalibrationCorners<2>;
        template class DrawCalibrationCorners<3>;
        template class DrawCalibrationCorners<4>;
        template class DrawCalibrationCorners<5>;
        template class DrawCalibrationCorners<6>;
        template class DrawCalibrationCorners<7>;
    }
}