//
// Created by geislerd on 27.04.17.
//

#ifndef ADTF_IO_CONVERTIMAGE_H
#define ADTF_IO_CONVERTIMAGE_H

#include <adtf/salbox/Image.h>
#include <utils/Matrix.h>

namespace saliency_sandbox {
    namespace adtf {
        template<typename _type>
        class ConvertImage {
        public:
            void convert(salbox::Image<_type> *in, saliency_sandbox::utils::IMatrix *out);
            void convert(saliency_sandbox::utils::IMatrix *in,salbox::Image<_type> *out);
        };
    }
}
#endif //ADTF_IO_CONVERTIMAGE_H
