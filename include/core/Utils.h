//
// Created by geislerd on 14.02.17.
//

#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <pthread.h>

namespace saliency_sandbox {

    namespace core {
        class Utils {
        public:
            static std::string uniqueName();
            static void setMainStackSize();
            static void setThreadStackSize(pthread_attr_t& attr);
            static void fps(void (*calc)(), float& fps);
        };

        template <typename _type>
        class TypeInfo {
        public:
            static const uint32_t SIZE = sizeof(_type);
        };

        template <>
        class TypeInfo<void> {
        public:
            static const uint32_t SIZE = 0;
        };
    }
}

#endif //UTILS_H
