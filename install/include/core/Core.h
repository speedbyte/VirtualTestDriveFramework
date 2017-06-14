//
// Created by geislerd on 28.03.17.
//

#ifndef CORE_H
#define CORE_H

#include <core/PropertyMap.h>
#include <stddef.h>

namespace saliency_sandbox {
    namespace core {

        class IProcessable;
        class IPort;
        class INode;
        class IBusChannel;
        class IBus;

        class IProcessable {
        public:

            virtual void process(time_t time) = 0;

            virtual time_t time() = 0;

            virtual bool eof() = 0;

            virtual float fps() = 0;
        };

        class IBusChannel {
        public:

            virtual const char *name() = 0;

            virtual void name(const char *name) = 0;

            virtual size_t size() = 0;

            virtual void *data() = 0;

            virtual void data(void *ptr) = 0;
        };

        class IBus {
        public:

            virtual uint32_t numChannels() = 0;

            virtual IBusChannel* channel(uint32_t index) = 0;
        };

        class IPort : public IProcessable , public IBusChannel {
        public:

            virtual IPort *connect() = 0;

            virtual void connect(IPort *port) = 0;

            virtual IProcessable* dependency() = 0;

            virtual INode* node() = 0;
        };

        class INode : public IProcessable {

        public:
            virtual uint32_t numInput() = 0;

            virtual uint32_t numOutput() = 0;

            virtual IPort *input(uint32_t index) = 0;

            virtual IPort *output(uint32_t index) = 0;

            virtual PropertyMap *properties() = 0;

            virtual const char* name() = 0;

            virtual void name(const char* name) = 0;
        };

        typedef saliency_sandbox::core::INode* (*new_pipeline_fun_ptr)(void);
    }
}

#endif //CORE_H
