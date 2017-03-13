//
// Created by geislerd on 08.02.17.
//

#ifndef PROCESSABLE_H
#define PROCESSABLE_H

#include <ctime>

namespace saliency_sandbox {

    namespace core {

        class IProcessable {
        public:

            virtual void process(time_t time) = 0;

            virtual time_t time() = 0;

            virtual bool eof() = 0;

            virtual float fps() = 0;
        };

    }
}

#endif //PROCESSABLE_H
