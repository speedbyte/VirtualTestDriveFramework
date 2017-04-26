//
// Created by geislerd on 16.03.17.
//

#include <kitti/DrawTracklet.h>

namespace saliency_sandbox {
    namespace kitti {

        template<Camera _camera>
        void DrawTracklet<_camera>::convert(input_image_t &img) {
            this->m_data.mat(img);
        }

        template<>
        void DrawTracklet<LEFT_GRAY>::convert(input_image_t &img) {
            cv::cvtColor(img, this->m_data, CV_GRAY2RGB);
        }

        template<>
        void DrawTracklet<RIGHT_GRAY>::convert(input_image_t &img) {
            cv::cvtColor(img, this->m_data, CV_GRAY2RGB);
        }

        template<Camera _camera>
        bool DrawTracklet<_camera>::inImage(cv::Point *points, size_t num) {
            bool ret = true;
            for (int i = 0; i < num && ret; i++)
                ret &= (points[i].x > 0 && points[i].y > 0) && (points[i].x < 1242 && points[i].y < 375);
            return ret;
        }

        template<Camera _camera>
        void DrawTracklet<_camera>::drawTracklet(Tracklet &tracklet, Calibration &calibration) {
            cv::Vec3f faces3D[16];
            cv::Vec2f faces2D[16];
            cv::Point pf[4], center(0, 0);
            size_t num = 0;
            std::stringstream saliency_ss;

            tracklet.boundingbox(faces3D);
            calibration.veloToCam(faces3D, faces2D, 16, _camera);

            for (int i = 0; i < 16; i += 4) {
                pf[0] = cv::Point2f(faces2D[i + 0]);
                pf[1] = cv::Point2f(faces2D[i + 1]);
                pf[2] = cv::Point2f(faces2D[i + 2]);
                pf[3] = cv::Point2f(faces2D[i + 3]);

                if (this->inImage(pf, 4)) {
                    cv::line(this->m_data, pf[0], pf[1], cv::Scalar(255, 0, 0));
                    cv::line(this->m_data, pf[1], pf[2], cv::Scalar(0, 255, 0));
                    cv::line(this->m_data, pf[2], pf[3], cv::Scalar(0, 0, 255));
                    cv::line(this->m_data, pf[3], pf[0], cv::Scalar(255, 0, 255));

                    center += pf[0] + pf[1] + pf[2] + pf[3];
                    num += 4;
                }

            }

            if (num > 0) {
                center.x /= num;
                center.y /= num;
            } else
                return;

            cv::putText(this->m_data, tracklet.label(), center, CV_FONT_HERSHEY_COMPLEX_SMALL, 0.8,
                        cvScalar(255, 255, 255), 1, CV_AA);

            if (tracklet.properties()->template has<float>("saliency")) {
                saliency_ss << "s: " << std::setprecision(2) << tracklet.properties()->template get<float>("saliency");
                cv::putText(this->m_data, saliency_ss.str(), cv::Point(center.x, center.y + 14),
                            CV_FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(255, 255, 255), 1, CV_AA);
            }

        }

        template<Camera _camera>
        void DrawTracklet<_camera>::drawTracklets(TrackletList &tracklets, Calibration &calibration) {
            for (int i = 0; i < tracklets.size(); i++)
                this->drawTracklet(*tracklets[i], calibration);
        }

        template<Camera _camera>
        DrawTracklet<_camera>::DrawTracklet() {
            this->template input<0>()->name("image");
            this->template input<1>()->name("tracking");
            this->template input<2>()->name("calibration");

            this->template output<0>()->name("image");
            this->template output<0>()->value(&(this->m_data));
        }

        template<Camera _camera>
        void DrawTracklet<_camera>::calc() {
            this->convert(*this->template input<0>()->value());
            this->drawTracklets(*this->template input<1>()->value(), *this->template input<2>()->value());

            this->template output<0>()->value(&(this->m_data));
        }

        template<Camera _camera>
        void DrawTracklet<_camera>::reset() {}

        template class DrawTracklet<LEFT_GRAY>;
        template class DrawTracklet<RIGHT_GRAY>;
        template class DrawTracklet<LEFT_RGB>;
        template class DrawTracklet<RIGHT_RGB>;
    }
}