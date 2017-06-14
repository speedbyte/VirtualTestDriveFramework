//
// Created by geislerd on 26.04.17.
//

#ifndef ADTF_IO_MEMORYREADER_H
#define ADTF_IO_MEMORYREADER_H

#include <core/Node.h>

namespace saliency_sandbox {
    namespace io {

        template<typename _in, typename _out>
        class MemoryReader : public saliency_sandbox::core::Node::template Input<_in>::template Output<_out> {
        protected:
            typedef _in Input;
            typedef _out Output;

        private:
            Output m_out;

        public:

            template<typename... _arg_t>
            MemoryReader(_arg_t... args) : m_out(args...) {
                this->template input<0>()->name("in");
                this->template input<0>()->checkDependencies(false);

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

#endif //ADTF_IO_MEMORYREADER_H
