//
// Created by geislerd on 26.04.17.
//

#ifndef ADTF_IO_TRACKEDOBJECTREADER_H
#define ADTF_IO_TRACKEDOBJECTREADER_H

#include <io/MemoryReader.h>
#include <adtf/salbox/TrackedObject.h>
#include <tracking/Tracklet.h>

namespace saliency_sandbox {
    namespace adtf {

        typedef std::vector<salbox::TrackedObject> TrackedObjectList;

        class TrackedObjectReader : public saliency_sandbox::io::MemoryReader<TrackedObjectList,saliency_sandbox::tracking::TrackletList> {
        public:
            TrackedObjectReader();

            void cvt(Input *in, Output *out) override;

            saliency_sandbox::tracking::Tracklet cvt(const salbox::TrackedObject& in);
        };
    }
}
#endif //ADTF_IO_TRACKEDOBJECTREADER_H
