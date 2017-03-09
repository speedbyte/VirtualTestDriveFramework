//
// Created by geislerd on 13.02.17.
//

#ifndef BUS_H
#define BUS_H

#include <cstdint>

namespace saliency_sandbox {

    namespace core {
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
    }
}

#endif //BUS_H
