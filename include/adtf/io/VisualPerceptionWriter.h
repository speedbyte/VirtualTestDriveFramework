//
// Created by geislerd on 26.04.17.
//

#ifndef ADTF_IO_VISUALPERCEPTIONWRITER
#define ADTF_IO_VISUALPERCEPTIONWRITER

#include <io/MemoryWriter.h>
#include <tracking/Tracklet.h>
#include <adtf/salbox/VisualPerception.h>

namespace saliency_sandbox {
    namespace adtf {

        typedef std::vector<salbox::VisualPerception> VisualPerceptionList;

        class VisualPerceptionWriter : public saliency_sandbox::io::MemoryWriter<saliency_sandbox::tracking::TrackletList,VisualPerceptionList> {
        public:
            VisualPerceptionWriter();

            void cvt(Input *in, Output *out) override;

            salbox::VisualPerception cvt(saliency_sandbox::tracking::Tracklet& in);
        };
    }
}
#endif //ADTF_IO_VISUALPERCEPTIONWRITER
