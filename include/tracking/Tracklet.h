//
// Created by geislerd on 26.04.17.
//

#ifndef TRACKING_TRACKLET_H
#define TRACKING_TRACKLET_H

#include <opencv2/core/core.hpp>
#include <core/PropertyMap.h>

namespace saliency_sandbox {
    namespace tracking {

        class Tracklet {
        private:
            cv::Vec3f m_position;
            cv::Vec3f m_orientation;
            cv::Vec3f m_dimension;

            saliency_sandbox::core::PropertyMap m_properties;
        public:

            saliency_sandbox::core::PropertyMap* properties();

            cv::Vec3f& position();
            cv::Vec3f& orientation();
            cv::Vec3f& dimension();

            void position(const cv::Vec3f& position);
            void orientation(const cv::Vec3f& position);
            void dimension(const cv::Vec3f& dimension);

            cv::Matx44f Rx();
            cv::Matx44f Ry();
            cv::Matx44f Rz();
            cv::Matx44f R();
            cv::Matx44f T();
            cv::Matx44f S();
            cv::Matx44f M();
        };

        typedef std::vector<Tracklet> TrackletList;

    }
}

#endif //TRACKING_TRACKLET_H
