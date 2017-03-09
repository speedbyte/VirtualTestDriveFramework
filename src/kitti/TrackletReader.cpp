//
// Created by geislerd on 21.02.17.
//

#include <kitti/TrackletReader.h>

namespace saliency_sandbox {
    namespace kitti {

        void Tracklet::calcRotation() {
            cv::Matx33f X, Y, Z;

            X = cv::Matx33f(1.0f, 0.0f, 0.0f, 0.0f, cosf(this->m_pose->rx), -sinf(this->m_pose->rx), 0.0f,
                            sinf(this->m_pose->rx), cosf(this->m_pose->rx));
            Y = cv::Matx33f(cosf(this->m_pose->ry), 0.0f, sinf(this->m_pose->ry), 0.0f, 1.0f, 0.0f,
                            -sinf(this->m_pose->ry), 0.0f, cosf(this->m_pose->ry));
            Z = cv::Matx33f(cosf(this->m_pose->rz), -sinf(this->m_pose->rz), 0.0f, sinf(this->m_pose->rz),
                            cosf(this->m_pose->rz), 0.0f, 0.0f, 0.0f, 1.0f);
            this->m_rotation = Z * Y * X;
        }

        void Tracklet::calcTranslation() {
            this->m_translation = cv::Vec3f(this->m_pose->tx, this->m_pose->ty, this->m_pose->tz);
        };

        Tracklet::Tracklet(Tracklets::tTracklet *tracklet) : m_tracklet(tracklet), m_pose(nullptr) {
            this->time(-1);
        }

        time_t Tracklet::time() {
            return this->m_time;
        }

        bool Tracklet::time(time_t time) {
            int pose_idx;

            this->m_time = time;
            pose_idx = (int) time - this->m_tracklet->first_frame;

            if (pose_idx < 0 || pose_idx >= (int) this->m_tracklet->poses.size()) {
                this->m_pose = nullptr;
                return false;
            }

            this->m_pose = &(this->m_tracklet->poses[pose_idx]);
            this->calcRotation();
            this->calcTranslation();
            return true;
        }

        cv::Vec3f Tracklet::size() {
            return cv::Vec3f(this->m_tracklet->w, this->m_tracklet->h, this->m_tracklet->l);
        }

        cv::Vec3f Tracklet::translation() {
            return cv::Vec3f(this->m_pose->tx, this->m_pose->ty, this->m_pose->tz);
        }

        cv::Vec3f Tracklet::rotation() {
            return cv::Vec3f(this->m_pose->rx, this->m_pose->ry, this->m_pose->rz);
        }

        void Tracklet::points(cv::Vec3f *points) {
            points[0] = this->m_translation + (this->m_rotation *
                                               cv::Vec3f(this->m_tracklet->l / 2.0f, this->m_tracklet->w / 2.0f,
                                                         0.0f));
            points[1] = this->m_translation + (this->m_rotation *
                                               cv::Vec3f(this->m_tracklet->l / 2.0f, -this->m_tracklet->w / 2.0f,
                                                         0.0f));
            points[2] = this->m_translation + (this->m_rotation *
                                               cv::Vec3f(-this->m_tracklet->l / 2.0f, -this->m_tracklet->w / 2.0f,
                                                         0.0f));
            points[3] = this->m_translation + (this->m_rotation *
                                               cv::Vec3f(-this->m_tracklet->l / 2.0f, this->m_tracklet->w / 2.0f,
                                                         0.0f));
            points[4] = this->m_translation + (this->m_rotation *
                                               cv::Vec3f(this->m_tracklet->l / 2.0f, this->m_tracklet->w / 2.0f,
                                                         this->m_tracklet->h));
            points[5] = this->m_translation + (this->m_rotation *
                                               cv::Vec3f(this->m_tracklet->l / 2.0f, -this->m_tracklet->w / 2.0f,
                                                         this->m_tracklet->h));
            points[6] = this->m_translation + (this->m_rotation *
                                               cv::Vec3f(-this->m_tracklet->l / 2.0f, -this->m_tracklet->w / 2.0f,
                                                         this->m_tracklet->h));
            points[7] = this->m_translation + (this->m_rotation *
                                               cv::Vec3f(-this->m_tracklet->l / 2.0f, this->m_tracklet->w / 2.0f,
                                                         this->m_tracklet->h));
        }

        void Tracklet::boundingbox(cv::Vec3f *faces) {
            cv::Vec3f points[8];

            this->points(points);
            faces[0] = points[0];
            faces[1] = points[1];
            faces[2] = points[5];
            faces[3] = points[4];

            faces[4] = points[1];
            faces[5] = points[2];
            faces[6] = points[6];
            faces[7] = points[5];

            faces[8] = points[2];
            faces[9] = points[3];
            faces[10] = points[7];
            faces[11] = points[6];

            faces[12] = points[3];
            faces[13] = points[0];
            faces[14] = points[4];
            faces[15] = points[7];
        }

        std::string Tracklet::label() {
            return this->m_tracklet->objectType;
        }

        void TrackletList::time(time_t time) {
            this->m_active_tracklets.clear();
            for (int i = 0; i < this->m_tracklets.size(); i++) {
                if (this->m_tracklets[i].time(time))
                    this->m_active_tracklets.push_back(&this->m_tracklets[i]);
            }
        }

        void TrackletList::push(Tracklets::tTracklet *tracklet) {
            this->m_tracklets.push_back(Tracklet(tracklet));
        }

        size_t TrackletList::size() {
            return this->m_active_tracklets.size();
        }

        Tracklet *TrackletList::operator[](uint32_t index) {
            return this->m_active_tracklets[index];
        }

        TrackletReader::TrackletReader(boost::filesystem::path path) : m_path(path) {
            this->reset();
        }

        void TrackletReader::calc() {
            this->m_data.time(this->time());
            this->output()->value(&this->m_data);
        }

        void TrackletReader::reset() {
            boost::filesystem::path labels;

            sserr << sspdir(this->m_path) << ssthrow;

            labels = this->m_path / "tracklet_labels.xml";
            sserr << sspfile(labels) << ssthrow;

            sserr << sscond(!this->m_tracklets.loadFromFile(labels.string()))
                  << "error while reading tracklets archive: " << labels << ssthrow;

            for (int i = 0; i < this->m_tracklets.numberOfTracklets(); i++)
                this->m_data.push(this->m_tracklets.getTracklet(i));
        }
    }
}