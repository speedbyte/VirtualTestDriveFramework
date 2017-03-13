//
// Created by geislerd on 25.02.17.
//

#include <core/Node.h>

#ifndef FPS_H
#define FPS_H
namespace saliency_sandbox {
    namespace utils {

        template<uint32_t _n>
        class _FPSCounter : public saliency_sandbox::core::Node::Input<void>::Output<float> {
        private:
            float m_fps;
            float m_x[_n];
            float* m_xc;
            clock_t m_clock;

        public:

            _FPSCounter() {
                this->reset();
                this->input()->checkPortSize(false);
                this->input()->name("process");
                this->output()->value(&(this->m_fps));
                this->output()->name("fps");
            }

            void calc() override {
                int j;
                float avg;

                avg = 0.0f;

                *this->m_xc = this->input()->fps();
                this->m_xc++;
                if(this->m_xc >= &(this->m_x[_n]))
                    this->m_xc = this->m_x;

                for(int i = 0; i < _n; i++) {
                    if(this->m_x[i] == 0.0f)
                        continue;
                    avg += this->m_x[i];
                    j++;
                }
                avg /= j;
                this->m_fps = avg;

                this->output()->value(&(this->m_fps));
            }

            void reset() override {
                this->m_fps = 0.0f;
                this->m_xc = this->m_x;
                this->m_clock = clock();
                for(int i; i < _n; this->m_x[i++] = 0.0f);
            }
        };

        typedef _FPSCounter<20> FPSCounter;
    }
}
#endif //FPS_H
