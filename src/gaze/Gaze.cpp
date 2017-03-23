//
// Created by geislerd on 22.03.17.
//

#include <gaze/Gaze.h>

namespace saliency_sandbox {
    namespace gaze {
        Gaze::Gaze(cv::Vec3f origin, cv::Vec2f angle) :
                m_origin(origin),
                m_angle(angle) {

        }

        Gaze::Gaze(cv::Point3f origin, cv::Vec2f angle) :
                m_origin(origin.x,origin.y,origin.z),
                m_angle(angle) {

        }

        Gaze::Gaze(cv::Vec3f origin, cv::Vec3f direction) :
                m_origin(origin),
                m_angle(origin.val[2] / sqrtf(origin.val[0]*origin.val[0]+origin.val[1]*origin.val[1]+origin.val[2]*origin.val[2]),atan2f(origin.val[1],origin.val[0])){

        }

        Gaze::Gaze(cv::Point3f origin, cv::Vec3f direction) :
                m_origin(origin),
                m_angle(origin.z / sqrtf(origin.x*origin.x+origin.y*origin.y+origin.z*origin.z),atan2f(origin.y,origin.x)) {

        }

        Gaze::Gaze(float x, float y, float z, float inclination, float azimuth) :
                m_origin(x,y,z),
                m_angle(inclination,azimuth) {

        }

        Gaze::Gaze(float x, float y, float z, float dx, float dy, float dz) :
                m_origin(x,y,z),
                m_angle(dz / sqrtf(dx*dx+dy*dy+dz*dz),atan2f(dy,dx)) {

        }

        cv::Vec3f& Gaze::origin() {
            return this->m_origin;
        }

        cv::Vec2f& Gaze::angle() {
            return this->m_angle;
        }

#define WRAP2PI(r) float(fmod(r,float(M_PI)))

        Gaze& Gaze::operator+=(const Gaze& gaze) {
            this->m_angle += gaze.m_angle;
            this->m_origin += gaze.m_origin;

            this->m_angle.val[0] = WRAP2PI(this->m_angle.val[0]);
            this->m_angle.val[1] = WRAP2PI(this->m_angle.val[1]);
            return *this;
        }

        cv::Vec3f Gaze::dir() {
            cv::Vec3f d;

            d.val[0] = sinf(this->m_angle.val[0])*cosf(this->m_angle.val[1]);
            d.val[1] = sinf(this->m_angle.val[0])*sinf(this->m_angle.val[1]);
            d.val[2] = cosf(this->m_angle.val[0]);
            return d;
        }

        template<>
        Gaze::Selector<Gaze::ORIGIN_X>::Selector() {
            this->template output<0>()->name("gaze");
            this->template input<0>()->name("x");
        };

        template<>
        Gaze::Selector<Gaze::ORIGIN_Y>::Selector() {
            this->template output<0>()->name("gaze");
            this->template input<0>()->name("y");
        };

        template<>
        Gaze::Selector<Gaze::ORIGIN_Z>::Selector() {
            this->template output<0>()->name("gaze");
            this->template input<0>()->name("z");
        };

        template<>
        Gaze::Selector<Gaze::ANGLE_INCLINATION>::Selector() {
            this->template output<0>()->name("gaze");
            this->template input<0>()->name("inclination");
        };

        template<>
        Gaze::Selector<Gaze::ANGLE_AZIMUTH>::Selector() {
            this->template output<0>()->name("gaze");
            this->template input<0>()->name("azimuth");
        };

        template<>
        void Gaze::Selector<Gaze::ORIGIN_X>::calc() {
            this->template output<0>()->value(&(this->template input<0>()->value()->m_origin.val[0]));
        };

        template<>
        void Gaze::Selector<Gaze::ORIGIN_Y>::calc() {
            this->template output<0>()->value(&(this->template input<0>()->value()->m_origin.val[1]));
        };

        template<>
        void Gaze::Selector<Gaze::ORIGIN_Z>::calc() {
            this->template output<0>()->value(&(this->template input<0>()->value()->m_origin.val[2]));
        };

        template<>
        void Gaze::Selector<Gaze::ANGLE_INCLINATION>::calc() {
            this->template output<0>()->value(&(this->template input<0>()->value()->m_angle.val[0]));
        };

        template<>
        void Gaze::Selector<Gaze::ANGLE_AZIMUTH>::calc() {
            this->template output<0>()->value(&(this->template input<0>()->value()->m_angle.val[1]));
        };

        template<Gaze::Component _component>
        void Gaze::Selector<_component>::reset() {};

        template class Gaze::Selector<Gaze::ORIGIN_X>;
        template class Gaze::Selector<Gaze::ORIGIN_Y>;
        template class Gaze::Selector<Gaze::ORIGIN_Z>;
        template class Gaze::Selector<Gaze::ANGLE_INCLINATION>;
        template class Gaze::Selector<Gaze::ANGLE_AZIMUTH>;
    }
}