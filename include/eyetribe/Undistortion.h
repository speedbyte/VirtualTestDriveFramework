//
// Created by geislerd on 13.04.17.
//

#ifndef EYETRIBE_CAMERADISTORTION_H
#define EYETRIBE_CAMERADISTORTION_H

#include <eyetribe/CameraCalibration.h>

namespace saliency_sandbox {
    namespace eyetribe {

        template<typename _type>
        class Undistortion : public saliency_sandbox::core::Node::
        template Input<_type, CameraMatrix, CameraDistortion>::
        template Output<_type> {
        private:
            _type m_output;

        protected:
            _type& distorted();
            CameraMatrix& matrix();
            CameraDistortion& distortion();
            _type& undistorted();

        public:
            Undistortion();
            void reset() override;
        };

        template<uint32_t _format>
        class CameraUndistortion : public Undistortion<typename VideoReader<_format>::Image> {
        public:
            void calc() override;
        };

        class PointsUndistortion : public Undistortion<CameraCorners> {
        public:
            void calc() override;
        };
    }
}
#endif //EYETRIBE_CAMERADISTORTION_H
