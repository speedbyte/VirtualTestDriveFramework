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
        };
    }
}

#endif //UTILS_H
