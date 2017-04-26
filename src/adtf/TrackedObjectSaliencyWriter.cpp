//
// Created by geislerd on 27.04.17.
//

#include <adtf/io/TrackedObjectSaliencyWriter.h>

namespace saliency_sandbox {
    namespace adtf {
        TrackedObjectSaliencyWriter::TrackedObjectSaliencyWriter() : MemoryWriter() { };

        void TrackedObjectSaliencyWriter::cvt(Input *in, Output *out) {
            out->clear();
            for(Input::iterator i = in->begin(); i != in->end(); ++i)
                out->push_back(this->cvt(*i));
        }

        salbox::TrackedObjectSaliency TrackedObjectSaliencyWriter::cvt(saliency_sandbox::tracking::Tracklet& in) {
            salbox::TrackedObjectSaliency out;

            out.id = uint32_t(in.properties()->template get<int>("id",0));
            out.max = double(in.properties()->template get<float>("saliency_max",0.0f));
            out.mean = double(in.properties()->template get<float>("saliency_mean",0.0f));
            out.integral = double(in.properties()->template get<float>("saliency_integral",0.0f));
            out.numPixels = size_t(in.properties()->template get<int>("num_pixels",0));
            out.maxRel = double(in.properties()->template get<float>("saliency_max_rel",0.0f));
            out.meanRel = double(in.properties()->template get<float>("saliency_mean_rel",0.0f));

            return out;
        };
    }
}