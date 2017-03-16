//
// Created by geislerd on 15.03.17.
//

#include <flow/Flow.h>

#ifndef OPTICALFLOW_H
#define OPTICALFLOW_H
namespace saliency_sandbox {
    namespace saliency {
        template<uint32_t _width, uint32_t _height, typename... _input>
        class _OpticalFlow : public _Flow<_width, _height,2> {

        };
    }
}
#endif //OPTICALFLOW_H
