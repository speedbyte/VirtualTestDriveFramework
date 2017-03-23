//
// Created by geislerd on 16.03.17.
//

#include <kitti/Velodyne2PolarImage.h>

namespace saliency_sandbox {
    namespace kitti {

        template<uint32_t _num_points>
        _Velodyne2PolarImage<_num_points>::_Velodyne2PolarImage() {
            this->template output<0>()->name("distance");
            this->template output<1>()->name("reflection");

            this->template output<0>()->value(&(this->m_distance));
            this->template output<1>()->value(&(this->m_reflection));

            this->template input<0>()->name("velodyne");
        }

        template<uint32_t _num_points>
        void _Velodyne2PolarImage<_num_points>::calc() {
            cv::Mat4f in;
            float x, y, z, w, r;
            cv::Point c;
            cv::Vec4f *v;
            const float rad2deg = ((360.0f) / (2.0f * float(M_PI)));

            // get opencv input accessor
            in = this->template input<0>()->value()->mat();

            // clear output
            this->m_distance.setTo(0.0f);
            this->m_reflection.setTo(0.0f);

            for (int i = 0; i < in.rows; i++) {
                v = in.ptr<cv::Vec4f>(i);
                x = v->val[0];
                y = v->val[1];
                z = v->val[2];
                w = v->val[3];
                r = sqrtf(x * x + y * y + z * z);
                c.y = ((int) (acosf(z / r) * rad2deg)) ;//% 180;
                c.x = ((int) ((atan2f(y, x) /*+ M_PI*/) * rad2deg)) ;//% 360;
                this->m_distance(c) = r;
                this->m_reflection(c) = w;
            }

            // refresh output pointer
            this->template output<0>()->value(&(this->m_distance));
            this->template output<1>()->value(&(this->m_reflection));
        }

        template<uint32_t _num_points>
        void _Velodyne2PolarImage<_num_points>::reset() {

        }

        template class _Velodyne2PolarImage<VELODYNE_BUFFER_SIZE>;
    }
}