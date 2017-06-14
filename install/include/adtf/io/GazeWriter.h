//
// Created by geislerd on 26.04.17.
//

#ifndef ADTF_IO_GAZEWRITER_H
#define ADTF_IO_GAZEWRITER_H

#include <gaze/GazeMovement.h>
#include <adtf/salbox/Gaze.h>
#include <io/MemoryWriter.h>
#include <tracking/Tracklet.h>

namespace saliency_sandbox {
    namespace adtf {

        class GazeMovementWriter : public saliency_sandbox::io::MemoryWriter<saliency_sandbox::gaze::GazeMovement,salbox::GazeMovementClassifier> {
        public:
            GazeMovementWriter();

            void cvt(Input *in, Output *out) override;
        };

        typedef std::vector<salbox::GazeTracking> GazeTrackingList;

        class GazeTrackingWriter : public saliency_sandbox::io::MemoryWriter<saliency_sandbox::tracking::TrackletList,GazeTrackingList> {
        public:
            GazeTrackingWriter();

            void cvt(Input *in, Output *out) override;

            salbox::GazeTracking cvt(saliency_sandbox::tracking::Tracklet& in);
        };
    }
}
#endif //ADTF_IO_GAZEWRITER_H
