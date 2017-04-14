//
// Created by geislerd on 14.04.17.
//

#include <eyetribe/Undistortion.h>

namespace saliency_sandbox {
    namespace eyetribe {
        template<typename _type>
        Undistortion<_type>::Undistortion() {
            this->template input<0>()->name("distorted");
            this->template input<1>()->name("matrix");
            this->template input<2>()->name("distortion");

            this->template output<0>()->name("undistorted");
            this->template output<0>()->value(&this->m_output);
        }

        template<typename _type>
        void Undistortion<_type>::reset() {

        }

        template<typename _type>
        _type& Undistortion<_type>::distorted() {
            return *this->template input<0>()->value();
        }

        template<typename _type>
        CameraMatrix& Undistortion<_type>::matrix() {
            return *this->template input<1>()->value();
        }

        template<typename _type>
        CameraDistortion& Undistortion<_type>::distortion() {
            return *this->template input<2>()->value();
        }

        template<typename _type>
        _type& Undistortion<_type>::undistorted() {
            return *this->template output<0>()->value();
        }

        template<uint32_t _format>
        void CameraUndistortion<_format>::calc() {
            cv::undistort(this->distorted(),this->undistorted(),this->matrix(),this->distortion());
        }

        void PointsUndistortion::calc() {
            CameraCorners& cc0 = this->distorted();
            CameraCorners& cc1 = this->undistorted();
            cv::Matx33f mat = cv::Matx33f(this->matrix().mat());
            bool backproject = this->properties()->template get<bool>("backproject",true);

            if(cc0.size() > 0) {
                cv::undistortPoints(cc0, cc1, mat, this->distortion());
                for(int i = 0; i < cc1.size() && backproject; i++)
                    cc1[i] = cv::Vec2f((mat * cv::Vec3f(cc1[i].val[0],cc1[i].val[1],1.0f)).val);
            } else
                this->undistorted().clear();
        }

        template class Undistortion<CameraCorners>;

        template class Undistortion<Format<0>::Image>;
        template class Undistortion<Format<1>::Image>;
        template class Undistortion<Format<2>::Image>;
        template class Undistortion<Format<3>::Image>;
        template class Undistortion<Format<4>::Image>;
        template class Undistortion<Format<5>::Image>;
        template class Undistortion<Format<6>::Image>;
        template class Undistortion<Format<7>::Image>;

        template class CameraUndistortion<0>;
        template class CameraUndistortion<1>;
        template class CameraUndistortion<2>;
        template class CameraUndistortion<3>;
        template class CameraUndistortion<4>;
        template class CameraUndistortion<5>;
        template class CameraUndistortion<6>;
        template class CameraUndistortion<7>;
    }
}