//
// Created by geislerd on 26.04.17.
//

#include <tracking/Tracklet.h>

namespace saliency_sandbox {
    namespace tracking {
        saliency_sandbox::core::PropertyMap* Tracklet::properties() {
            return &this->m_properties;
        }

        cv::Vec3f& Tracklet::position() {
            return this->m_position;
        }

        cv::Vec3f& Tracklet::orientation() {
            return this->m_orientation;
        }

        cv::Vec3f& Tracklet::dimension() {
            return this->m_dimension;
        }

        void Tracklet::position(const cv::Vec3f& position) {
            this->m_position = position;
        }

        void Tracklet::orientation(const cv::Vec3f& position) {
            this->m_orientation = position;
        }

        void Tracklet::dimension(const cv::Vec3f& dimension) {
            this->m_dimension = dimension;
        }

        cv::Matx44f Tracklet::Rx() {
            const float cosa = cosf(this->m_dimension.val[0]);
            const float sina = sinf(this->m_dimension.val[0]);

            return cv::Matx44f(
                    +1.0f, +0.0f, +0.0f, +0.0f,
                    +0.0f, +cosa, -sina, +0.0f,
                    +0.0f, +sina, +cosa, +0.0f,
                    +0.0f, +0.0f, +0.0f, +1.0f);
        }

        cv::Matx44f Tracklet::Ry() {
            const float cosa = cosf(this->m_dimension.val[1]);
            const float sina = sinf(this->m_dimension.val[1]);

            return cv::Matx44f(
                    +cosa, +0.0f, +sina, +0.0f,
                    +0.0f, +1.0f, +0.0f, +0.0f,
                    -sina, +0.0f, +cosa, +0.0f,
                    +0.0f, +0.0f, +0.0f, +1.0f);
        }

        cv::Matx44f Tracklet::Rz() {
            const float cosa = cosf(this->m_dimension.val[2]);
            const float sina = sinf(this->m_dimension.val[2]);

            return cv::Matx44f(
                    +cosa, -sina, +0.0f, +0.0f,
                    +sina, +cosa, +0.0f, +0.0f,
                    +0.0f, +0.0f, +1.0f, +0.0f,
                    +0.0f, +0.0f, +0.0f, +1.0f);
        }

        cv::Matx44f Tracklet::R() {
            //TODO: check rotation order
            return this->Rx()*this->Ry()*this->Rz();
        }

        cv::Matx44f Tracklet::T() {
            const float posx = this->m_position.val[0];
            const float posy = this->m_position.val[1];
            const float posz = this->m_position.val[2];

            return cv::Matx44f(
                    +0.0f, +0.0f, +0.0f, +posx,
                    +0.0f, +0.0f, +0.0f, +posy,
                    +0.0f, +0.0f, +1.0f, +posz,
                    +0.0f, +0.0f, +0.0f, +1.0f);
        }

        cv::Matx44f Tracklet::S() {
            const float scax = this->m_dimension.val[0];
            const float scay = this->m_dimension.val[1];
            const float scaz = this->m_dimension.val[2];

            return cv::Matx44f(
                    +scax, +0.0f, +0.0f, +0.0f,
                    +0.0f, +scay, +0.0f, +0.0f,
                    +0.0f, +0.0f, +scaz, +0.0f,
                    +0.0f, +0.0f, +0.0f, +1.0f);
        }

        cv::Matx44f Tracklet::M() {
            return this->T() * this->R() * this->S();
        }
    }
}