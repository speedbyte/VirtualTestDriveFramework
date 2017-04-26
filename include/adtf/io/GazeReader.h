//
// Created by geislerd on 26.04.17.
//

#ifndef ADTF_IO_GAZEREADER_H
#define ADTF_IO_GAZEREADER_H

#include <io/MemoryReader.h>
#include <adtf/salbox/Gaze.h>
#include <gaze/Gaze.h>

namespace saliency_sandbox {
    namespace adtf {

        class GazeReader : public saliency_sandbox::io::MemoryReader<salbox::Gaze,saliency_sandbox::gaze::Gaze> {
        public:
            GazeReader();

            void cvt(Input *in, Output *out) override;
        };
    }
}
#endif //ADTF_IO_GAZEREADER_H
