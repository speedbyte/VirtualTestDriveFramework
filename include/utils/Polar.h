//
// Created by geislerd on 23.03.17.
//

#ifndef POLAR_H
#define POLAR_H

#include <utils/Image.h>

namespace saliency_sandbox {
    namespace utils {

        typedef _HeatmapImage<360,180> PolarHeatmapImage;
        typedef _IntensityImage<360,180> PolarIntensityImage;
        typedef _BinaryImage<360,180> PolarBinaryImage;

    }
}

#endif //POLAR_H
