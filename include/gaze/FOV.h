//
// Created by geislerd on 23.03.17.
//

#ifndef FOV_H
#define FOV_H

#include <core/Node.h>
#include <utils/Image.h>
#include <gaze/Gaze.h>
#include <utils/Polar.h>

namespace saliency_sandbox {
    namespace gaze {

        class FOV : public saliency_sandbox::core::Node::
        template Input<Gaze,saliency_sandbox::utils::PolarHeatmapImage>::
        template Output<saliency_sandbox::utils::PolarHeatmapImage> {
        private:
            saliency_sandbox::utils::PolarHeatmapImage m_fov;

        public:

            /**
             * Initialize FOV node
             */
            FOV();

            /**
             * Calculates the dot product of two vectors x and y
             * @param x first vector
             * @param y second vector
             * @return dot product of vector x and y
             */
            float dot(cv::Vec3f x, cv::Vec3f y);

            /**
             * Calculate the distance between a line described by point x1 to x2
             * and a point x0
             * @param x1 first point of line
             * @param x2 second point of line
             * @param x0 point to calculate distance
             * @return distance between line (x1->x2) and point x0
             */
            float dist(cv::Vec3f x1, cv::Vec3f x2, cv::Vec3f x0);

            /**
             * Calculate the gaussian weight of distance by a given sigma and mean = 0
             * @param dist distance to weight
             * @param sigma deviation of the gaussian distribution
             * @return weight of distance
             */
            float weight(float dist, float sigma);

            void calc() override;

            void reset() override;
        };

    }
}

#endif //FOV_H
