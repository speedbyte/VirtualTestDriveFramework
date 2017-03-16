//
// Created by geislerd on 15.03.17.
//

#ifndef TRACKING_H
#define TRACKING_H

#include <utils/Image.h>

namespace saliency_sandbox {
    namespace saliency {

        template<uint32_t _width, uint32_t _height, typename... _input>
        class Tracking : public Saliency<_width,_height,saliency_sandbox::utils::_HeatmapImage<_width,_height>,saliency_sandbox::flow::_FlowField<_width,_height>> {

        };
    }
}

#endif //TRACKING_H
