//
// Created by geislerd on 27.04.17.
//

#include <adtf/io/VisualPerceptionWriter.h>

namespace saliency_sandbox {
    namespace adtf {
        VisualPerceptionWriter::VisualPerceptionWriter() : MemoryWriter() { };

        void VisualPerceptionWriter::cvt(Input *in, Output *out) {
            out->clear();
            for(Input::iterator i = in->begin(); i != in->end(); ++i)
                out->push_back(this->cvt(*i));
        }

        salbox::VisualPerception VisualPerceptionWriter::cvt(saliency_sandbox::tracking::Tracklet& in) {
            salbox::VisualPerception out;

            out.id = uint32_t(in.properties()->template get<int>("id",0));
            out.probability = in.properties()->template get<float>("perception_probability",0.0f);
            out.memory = in.properties()->template get<float>("perception_memory",0.0f);

            return out;
        };
    }
}