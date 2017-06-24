//
// Created by veikas on 23.06.17.
//

#ifndef SALIENCYSANDBOX2_0_NOISE_H
#define SALIENCYSANDBOX2_0_NOISE_H

#include <kitti/TrackletReader.h>

namespace saliency_sandbox {
    namespace sensor_fusion {

        typedef std::vector<saliency_sandbox::kitti::Tracklet> TrackletList;
        // Input is
        // Output is a std::vector type
        class Noise : public saliency_sandbox::core::Node::template Input<saliency_sandbox::kitti::TrackletList>::template Output<TrackletList> {
        private:
            // The tracklet list after noise induction
            TrackletList m_out;

            //member_function
            saliency_sandbox::kitti::Tracklet noise(const saliency_sandbox::kitti::Tracklet& tracklet) {
                return tracklet;
            }
        public:

            Noise() {  // Direct Initialisation
                this->template input<0>()->name("tracklets");
                this->template output<0>()->name("noise");
                this->template output<0>()->value(&this->m_out);
            }

            void calc() override {
                saliency_sandbox::kitti::TrackletList& tl = *this->template input<0>()->value();
                this->m_out.clear();
                for(int i = 0; i < tl.size(); i++)
                    this->m_out.push_back(this->noise(*tl[i]));
            }

            void reset() override {

            }
        };
    }
}
#endif //SALIENCYSANDBOX2_0_NOISE_H
