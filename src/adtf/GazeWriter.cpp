//
// Created by geislerd on 27.04.17.
//

#include <adtf/io/GazeWriter.h>

namespace saliency_sandbox {
    namespace adtf {
        GazeMovementWriter::GazeMovementWriter() : MemoryWriter() { };

        void GazeMovementWriter::cvt(Input *in, Output *out) {
            switch(in->type()) {
                case saliency_sandbox::gaze::GazeMovement::FIXATION:
                    out->gazeClass = salbox::GazeMovementClassifier::GC_FIXATION;
                    break;
                case saliency_sandbox::gaze::GazeMovement::SACCADE:
                    out->gazeClass = salbox::GazeMovementClassifier::GC_SACCADE;
                    break;
                case saliency_sandbox::gaze::GazeMovement::UNKNOWN:
                default:
                    out->gazeClass = salbox::GazeMovementClassifier::GC_UNKNOWN;
                    break;
            }

            if(     in->prev() == nullptr ||
                    in->prev()->type() == saliency_sandbox::gaze::GazeMovement::UNKNOWN ||
                    (
                            in->prev()->gaze().angle().val[0] == 0.0f  &&
                            in->prev()->gaze().angle().val[1] == 0.0f  &&
                            in->prev()->gaze().origin().val[0] == 0.0f &&
                            in->prev()->gaze().origin().val[1] == 0.0f &&
                            in->prev()->gaze().origin().val[2] == 0.0f
                    )) {
                out->angle = 0.0;
                out->angleSpeed = 0.0;
            } else {
                out->angle = cv::norm(in->gaze().angle()-in->prev()->gaze().angle());
                out->angleSpeed = out->angle / this->properties()->template get<float>("frequency",60.0f);
            }
        };

        GazeTrackingWriter::GazeTrackingWriter() : MemoryWriter() { };

        void GazeTrackingWriter::cvt(Input *in, Output *out) {
            out->clear();
            for(Input::iterator i = in->begin(); i != in->end(); ++i)
                out->push_back(this->cvt(*i));
        };

        salbox::GazeTracking GazeTrackingWriter::cvt(saliency_sandbox::tracking::Tracklet& in) {
            salbox::GazeTracking out;

            out.id = uint32_t(in.properties()->template get<int>("id",0));
            out.numFixations = size_t(in.properties()->template get<int>("fixation_num",0));
            out.lastFixation = int64_t(in.properties()->template get<int>("fixation_last",0));
            out.firstFixation = int64_t(in.properties()->template get<int>("fixation_first",0));
            out.maxDense = double(in.properties()->template get<float>("fixation_dense",0.0f));

            return out;
        };
    }
}