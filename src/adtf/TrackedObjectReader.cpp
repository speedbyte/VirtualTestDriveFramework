//
// Created by geislerd on 27.04.17.
//

#include <adtf/io/TrackedObjectReader.h>

namespace saliency_sandbox {
    namespace adtf {
        TrackedObjectReader::TrackedObjectReader() : MemoryReader() { };

        void TrackedObjectReader::cvt(Input *in, Output *out) {
            out->clear();
            for(Input::iterator i = in->begin(); i != in->end(); ++i)
                out->push_back(this->cvt(*i));
        }

        saliency_sandbox::tracking::Tracklet TrackedObjectReader::cvt(const salbox::TrackedObject& in) {
            saliency_sandbox::tracking::Tracklet out;

            out.position(cv::Vec3f(
                    float(in.position[0]),
                    float(in.position[1]),
                    float(in.position[2])));
            out.dimension(cv::Vec3f(
                    float(in.width),
                    float(in.length),
                    float(in.height)));
            out.orientation(cv::Vec3f(
                    float(in.orientation[2] /
                          sqrt(
                                  in.orientation[0]*in.orientation[0]+
                                  in.orientation[1]*in.orientation[1]+
                                  in.orientation[2]*in.orientation[2])),
                    float(atan2(
                            in.orientation[1],
                            in.orientation[0])),
                    0.0f)); //TODO: incomplete rotation - roll angle missing
            out.properties()->template set<int>("id",in.id);
            out.properties()->template set<int>("classification",in.classification);

            return out;
        };
    }
}