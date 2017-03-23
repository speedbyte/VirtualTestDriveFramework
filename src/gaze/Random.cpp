//
// Created by geislerd on 22.03.17.
//

#include <gaze/Random.h>


namespace saliency_sandbox {
    namespace gaze {

#define DEG2RAD(d) ((d)/360.0f)*float(M_PI)
#define SGN(x) (((x)<0.0f)?(-1.0f):(1.0f))

        Random::Random() :
                m_gaze(0.0f,0.0f,0.0f,0.0f,0.0f),
                m_gm(this->m_gaze),
                m_gms(),
                m_rng() {
            this->template output<0>()->name("gaze");
            this->template output<0>()->value(&this->m_gaze);
            this->template output<1>()->name("gaze movement");
            this->template output<1>()->value(&this->m_gm);
            this->reset();
        }

        Gaze Random::gaze(const float& inclination_rate, const float& azimuth_rate, const float& x_rate, const float& y_rate, const float& z_rate) {
            boost::random::normal_distribution<> inclination(0.0f,inclination_rate), azimuth(0.0f,azimuth_rate);
            boost::random::normal_distribution<> x(0.0f,x_rate), y(0.0f,y_rate), z(0.0f,z_rate);
            Gaze gaze(
                    float(x(this->m_rng)),
                    float(y(this->m_rng)),
                    float(z(this->m_rng)),
                    float(inclination(this->m_rng)),
                    float(azimuth(this->m_rng)));

            gaze.angle().val[0] *= -SGN(this->m_gaze.angle().val[0]*gaze.angle().val[0]);
            gaze.angle().val[1] *= -SGN(this->m_gaze.angle().val[1]*gaze.angle().val[1]);
            return gaze;
        }

        void Random::calc() {
            float fixation_prior, inclination_rate, azimuth_rate, origin_x_rate, origin_y_rate, origin_z_rate, inclination_offset, azimuth_offset;
            GazeMovement::Type type;
            boost::random::bernoulli_distribution<> fixation_saccade;
            size_t max_hist;

            // determine gaze movement type
            fixation_prior = this->properties()->template get<float>("fixation_prior",0.90f);
            fixation_saccade = boost::random::bernoulli_distribution<>(1.0f-fixation_prior);
            type = GazeMovement::Type(fixation_saccade(this->m_rng));

            switch(type) {
                case GazeMovement::FIXATION:
                    inclination_rate = this->properties()->template get<float>("fixation_inclination_rate",DEG2RAD(3.0f));
                    azimuth_rate = this->properties()->template get<float>("fixation_azimuth_rate",DEG2RAD(3.0f));
                    break;
                case GazeMovement::SACCADE:
                    inclination_rate = this->properties()->template get<float>("saccade_inclination_rate",DEG2RAD(3.0f));
                    azimuth_rate = this->properties()->template get<float>("saccade_azimuth_rate",DEG2RAD(90.0f));
                    break;
                default:
                    inclination_rate = 0.0f;
                    azimuth_rate = 0.0f;
                    sserr << "unknown gaze movement type" << ssthrow;
            }

            origin_x_rate = this->properties()->template get<float>("origin_x_rate",0.02f);
            origin_y_rate = this->properties()->template get<float>("origin_y_rate",0.02f);
            origin_z_rate = this->properties()->template get<float>("origin_z_rate",0.02f);

            // create new gaze
            inclination_offset = this->properties()->template get<float>("inclination_offset",DEG2RAD(180.0f-5.0f));
            azimuth_offset = this->properties()->template get<float>("azimuth_offset",DEG2RAD(0.0f));

            // remove inclination offset
            if(this->m_gaze.angle().val[0] != 0.0f)
                this->m_gaze.angle().val[0] -= inclination_offset;

            // remove atimuth offset
            if(this->m_gaze.angle().val[1] != 0.0f)
                this->m_gaze.angle().val[1] -= azimuth_offset;

            // add random gaze movement
            this->m_gaze += Random::gaze(inclination_rate,azimuth_rate,origin_x_rate,origin_y_rate,origin_z_rate);

            // add inclination offset
            this->m_gaze.angle().val[0] += inclination_offset;

            // add azimuth offset
            this->m_gaze.angle().val[1] += azimuth_offset;

            // store in history
            this->m_gms.push_back(this->m_gms.end()->next(this->m_gaze,type));

            // delete old elements from history
            max_hist = (size_t)this->properties()->template get<int>("history",500);
            while(this->m_gms.size() > max_hist)
                this->m_gms.erase(this->m_gms.begin());
            this->m_gms.begin()->prev(nullptr);

            // copy latest gaze movement to referenced output
            this->m_gm = *this->m_gms.end();
        }

        void Random::reset() {
            this->m_rng.seed(0xaffe);
        }
    }
}