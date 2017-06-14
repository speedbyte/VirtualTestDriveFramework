//
// Created by geislerd on 26.04.17.
//

#ifndef ADTF_IO_GAZEWRITER_H
#define ADTF_IO_GAZEWRITER_H

#include <io/MemoryWriter.h>
#include <adtf/salbox/TrackedObject.h>
#include <tracking/Tracklet.h>

namespace saliency_sandbox {
    namespace adtf {

        typedef std::vector<salbox::TrackedObjectSaliency> TrackedObjectList;

        class TrackedObjectSaliencyWriter : public saliency_sandbox::io::MemoryWriter<saliency_sandbox::tracking::TrackletList,TrackedObjectList> {
        public:
            TrackedObjectSaliencyWriter();

            void cvt(Input *in, Output *out) override;

            salbox::TrackedObjectSaliency cvt(saliency_sandbox::tracking::Tracklet& in);
        };
    }
}
#endif //ADTF_IO_GAZEWRITER_H
