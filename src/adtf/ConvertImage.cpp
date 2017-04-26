//
// Created by geislerd on 27.04.17.
//

#include <adtf/io/ConvertImage.h>

namespace saliency_sandbox {
    namespace adtf {

        template <typename _type>
        void ConvertImage<_type>::convert(salbox::Image<_type> *in, saliency_sandbox::utils::IMatrix *out) {
            if(in->width != out->width() || in->height != out->height() || in->size()*sizeof(_type) != out->bytes())
                return; //TODO: error handling

            memcpy(out->data(),in->buffer,out->bytes());
        };

        template <typename _type>
        void ConvertImage<_type>::convert(saliency_sandbox::utils::IMatrix *in,salbox::Image<_type> *out) {
            if(out->width != in->width() || out->height != in->height() || out->size()*sizeof(_type) != in->bytes())
                return; //TODO: error handling

            memcpy(out->buffer,in->data(),in->bytes());
        };

        template class ConvertImage<uint8_t>;
        template class ConvertImage<uint8_t[3]>;
        template class ConvertImage<float>;
    }
}