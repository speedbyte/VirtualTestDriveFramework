//
// Created by geislerd on 08.02.17.
//

#include <io/ImageMemoryWriter.h>

namespace saliency_sandbox {
    namespace io {

        ImageMemoryWriter::ImageMemoryWriter() {
            this->reset();

            // setup input port
            this->template input<0>()->name("image");
            this->template input<0>()->checkPortSize(false);

            this->template output<0>()->name("image");
            this->template output<0>()->value(&this->m_mat);
        }

        void ImageMemoryWriter::calc() {
            this->m_mat = this->template input<0>()->value()->mat();
        }

        void ImageMemoryWriter::reset() {

        }
    }
}