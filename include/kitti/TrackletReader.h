//
// Created by geislerd on 16.02.17.
//

#ifndef TRACKLETREADER_H
#define TRACKLETREADER_H

#include <core/Node.h>
#include <kitti/CalibrationReader.h>
#include <kitti/tracklets.h>

namespace saliency_sandbox {
    namespace kitti {

        class Tracklet {
        private:
            Tracklets::tTracklet* m_tracklet;
            Tracklets::tPose* m_pose;
            time_t m_time;
            cv::Matx33f m_rotation;
            cv::Vec3f m_translation;

            void calcRotation();

            void calcTranslation();

        public:
            Tracklet(Tracklets::tTracklet* tracklet);

            time_t time();

            bool time(time_t time);

            cv::Vec3f size();

            cv::Vec3f translation();

            cv::Vec3f rotation();

            void points(cv::Vec3f* points);

            void boundingbox(cv::Vec3f* faces);

            std::string label();

        };

        class TrackletList {
        private:
            std::vector<Tracklet> m_tracklets;
            std::vector<Tracklet*> m_active_tracklets;
        public:
            void time(time_t time);
            void push(Tracklets::tTracklet *tracklet);
            size_t size();
            Tracklet* operator[](uint32_t index);
        };

        class TrackletReader : public saliency_sandbox::core::Node::template Input<>::template Output<TrackletList> {
        private:
            boost::filesystem::path m_path;
            Tracklets m_tracklets;
            TrackletList m_data;

        public:
            TrackletReader(boost::filesystem::path path);

            void calc() override;

            void reset() override;
        };
    }
}
#endif //TRACKLETREADER_H
