//
// Created by veikas on 23.06.17.
//

#ifndef SALIENCYSANDBOX2_0_SENSORFUSION_H
#define SALIENCYSANDBOX2_0_SENSORFUSION_H

#include <sensor_fusion/Noise.h>

namespace saliency_sandbox {
    namespace sensor_fusion {
        class PriorityGraph : public saliency_sandbox::core::Node::template Input<
                TrackletList,
                TrackletList,
                TrackletList,
                TrackletList,
                TrackletList>::template Output<TrackletList> {
        private:
            TrackletList m_out;
        public:


            PriorityGraph() {
                this->template input<0>()->name("sensor 0");
                this->template input<1>()->name("sensor 1");
                this->template input<2>()->name("sensor 2");
                this->template input<3>()->name("sensor 3");
                this->template input<4>()->name("sensor 4");

                this->template output<0>()->name("tracklets");
                this->template output<0>()->value(&this->m_out);
            }

            void calc() override {
                const TrackletList& sensor0 = *this->template input<0>()->value();
                const TrackletList& sensor1 = *this->template input<1>()->value();
                const TrackletList& sensor2 = *this->template input<2>()->value();
                const TrackletList& sensor3 = *this->template input<3>()->value();
                const TrackletList& sensor4 = *this->template input<4>()->value();
            }

            void reset() override {

            }
        };
    }
}
#endif //SALIENCYSANDBOX2_0_SENSORFUSION_H

