//
// Created by geislerd on 27.04.17.
//
#include <adtf/io/GazeReader.h>

namespace saliency_sandbox {
    namespace adtf {

        GazeReader::GazeReader() : MemoryReader(0.0f,0.0f,0.0f,0.0f,0.0f) { };

        void GazeReader::cvt(Input *in, Output *out) {
            out->origin().val[0] = float(in->gazeOrigin[0]);
            out->origin().val[1] = float(in->gazeOrigin[1]);
            out->origin().val[2] = float(in->gazeOrigin[2]);
            out->angle().val[0] = float(
                    in->gazeDirection[2] /
                    sqrt(
                            in->gazeDirection[0]*in->gazeDirection[0]+
                            in->gazeDirection[1]*in->gazeDirection[1]+
                            in->gazeDirection[2]*in->gazeDirection[2]));
            out->angle().val[1] = float(
                    atan2(
                            in->gazeDirection[1],
                            in->gazeDirection[0]));
            //TODO: timestamp not considered in internal representation
        };
    }
}