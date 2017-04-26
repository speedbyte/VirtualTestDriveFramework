//
// Created by geislerd on 26.04.17.
//

#ifndef ADTF_IO_MEMORYWRITER_H
#define ADTF_IO_MEMORYWRITER_H

#include <core/Node.h>

namespace saliency_sandbox {
    namespace io {

        template<typename _in,typename _out>
        class MemoryWriter : public saliency_sandbox::core::Node::template Input<_in>::template Output<_out> {
        protected:
            typedef _out Output;
            typedef _in Input;

        private:
            Output m_out;

        public:

            template<typename... _arg_t>
            MemoryWriter(_arg_t... args) : m_out(args...) {
                this->template input<0>()->name("in");
                this->template output<0>()->name("out");
                this->template output<0>()->value(&this->m_out);
            }

            virtual void cvt(Input*,Output*) = 0;

            void calc() override {
                this->cvt(
                        this->template input<0>()->value(),
                        &this->m_out
                );
            }

            void reset() override { }
        };
    }
}

#endif //ADTF_IO_MEMORYWRITER_H
