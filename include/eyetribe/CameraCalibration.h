//
// Created by geislerd on 13.04.17.
//

#ifndef EYETRIBE_CAMERACALIBRATION_H
#define EYETRIBE_CAMERACALIBRATION_H

#include <core/Node.h>
#include <eyetribe/VideoReader.h>

#define EYETRIBE_CHECKERBOARD_ROWS 4
#define EYETRIBE_CHECKERBOARD_COLS 4
#define EYETRIBE_CHECKERBOARD_NUM (EYETRIBE_CHECKERBOARD_ROWS*EYETRIBE_CHECKERBOARD_COLS)
#define EYETRIBE_CHECKERBOARD_SIZE cv::Size(EYETRIBE_CHECKERBOARD_ROWS,EYETRIBE_CHECKERBOARD_COLS)
#define EYETRIBE_CHECKERBOARD_DISTANCE float(1.0f/(float(EYETRIBE_CHECKERBOARD_ROWS)-1.0f)*(40.0f/3.0f))
#define EYETRIBE_CHECKERBOARD_OBJECT_COORD(x,y) cv::Vec3f(EYETRIBE_CHECKERBOARD_DISTANCE*float(x),EYETRIBE_CHECKERBOARD_DISTANCE*float(y),0.0f)

namespace saliency_sandbox {
    namespace eyetribe {

        typedef saliency_sandbox::utils::_Matrix<3,3,float> CameraMatrix;
        typedef saliency_sandbox::utils::_Matrix<5,1,float> CameraDistortion;
        typedef std::vector<cv::Vec2f> CameraCorners;
        typedef std::vector<cv::Vec3f> ObjectCorners;
        typedef std::vector<CameraCorners> CameraCornersList;
        typedef std::vector<ObjectCorners> ObjectCornersList;

        template <uint32_t _format>
        class CameraCalibration : public saliency_sandbox::core::Node::
        template Input<typename VideoReader<_format>::Image>::
        template Output<CameraMatrix,CameraDistortion,bool,float,CameraCorners,bool> {
        private:
            CameraMatrix m_camera_matrix;
            CameraDistortion m_camera_distortion;
            CameraCorners m_camera_corners;
            CameraCorners m_camera_corners_mean;
            ObjectCorners m_object_corners;
            ObjectCornersList m_object_corners_list;
            CameraCornersList m_camera_corners_list;

            bool m_has_corners;
            bool m_is_calibrated;
            float m_coverage;
            float m_error;

        public:
            CameraCalibration() {
                this->template input<0>()->name("image");

                this->template output<0>()->name("matrix");
                this->template output<0>()->value(&this->m_camera_matrix);
                this->template output<1>()->name("distortion");
                this->template output<1>()->value(&this->m_camera_distortion);
                this->template output<2>()->name("valid");
                this->template output<2>()->value(&this->m_is_calibrated);
                this->template output<3>()->name("error");
                this->template output<3>()->value(&this->m_error);
                this->template output<4>()->name("corners");
                this->template output<4>()->value(&this->m_camera_corners);
                this->template output<5>()->name("valid");
                this->template output<5>()->value(&this->m_has_corners);

                this->reset();
            }

            float removePoints() {
                std::vector<int> camera_corner_hull;
                ObjectCornersList object_corners_list;
                CameraCornersList camera_corners_list;
                CameraCorners camera_corners_mean;
                int cidx;
                float cdist, tcdist, coverage;
                cv::Vec2f c;

                if(this->m_camera_corners_list.size() < 3)
                    return 0.0f;

                cv::convexHull(this->m_camera_corners_mean, camera_corner_hull);

                for(int i = 0; i < camera_corner_hull.size(); i++) {
                    camera_corners_list.push_back(this->m_camera_corners_list[camera_corner_hull[i]]);
                    object_corners_list.push_back(this->m_object_corners_list[camera_corner_hull[i]]);
                    camera_corners_mean.push_back(this->m_camera_corners_mean[camera_corner_hull[i]]);
                }

                coverage = float(cv::contourArea(camera_corners_mean)/float(Format<_format>::WIDTH*Format<_format>::HEIGHT));

                c = cv::Vec2f(Format<_format>::WIDTH/2.0f,Format<_format>::HEIGHT/2.0f);
                cdist = FLT_MAX;
                cidx = -1;

                for(int i = 1; i < this->m_camera_corners_mean.size(); i++) {
                    tcdist = (float)cv::norm(c,this->m_camera_corners_mean[i]);
                    if(tcdist < cdist) {
                        cidx = i;
                        cdist = tcdist;
                    }
                }
                if(cidx != -1) {
                    camera_corners_list.push_back(this->m_camera_corners_list[cidx]);
                    object_corners_list.push_back(this->m_object_corners_list[cidx]);
                    camera_corners_mean.push_back(this->m_camera_corners_mean[cidx]);
                }

                this->m_camera_corners_list = camera_corners_list;
                this->m_object_corners_list = object_corners_list;
                this->m_camera_corners_mean = camera_corners_mean;

                return coverage;
            }

            bool findCorners(cv::Mat1b in) {
                cv::Mat1b scaled;
                cv::Vec2f camera_corners_mean;

                cv::pyrDown(in,scaled);
                cv::pyrDown(scaled,scaled);

                this->m_has_corners = cv::findCirclesGrid(scaled,EYETRIBE_CHECKERBOARD_SIZE,this->m_camera_corners);

                if(!this->m_has_corners)
                    return false; //checkerboard not found

                for(int i = 0; i < EYETRIBE_CHECKERBOARD_NUM; i++) {
                    this->m_camera_corners[i] *= 4.0f;//2.0f;
                    camera_corners_mean += this->m_camera_corners[i] / EYETRIBE_CHECKERBOARD_NUM;
                }

                this->m_camera_corners_list.push_back(this->m_camera_corners);
                this->m_object_corners_list.push_back(this->m_object_corners);
                this->m_camera_corners_mean.push_back(camera_corners_mean);

                return true;
            }

            void calc() override {
                cv::Mat1b in;
                float coverage, min_coverage;

                in = this->template input<0>()->value()->mat();
                min_coverage = this->properties()->template get<float>("coverage",0.35f);

                if(!this->findCorners(in))
                    return;

                coverage = this->removePoints();
                if(this->m_coverage == coverage)
                    return; //nothing changed
                else
                    this->m_coverage = coverage;

                if(this->m_coverage < min_coverage)
                    return; //collect more data

                this->m_error = (float)cv::calibrateCamera(
                        this->m_object_corners_list,
                        this->m_camera_corners_list,
                        cv::Size(Format<_format>::WIDTH,Format<_format>::HEIGHT),
                        this->m_camera_matrix,
                        this->m_camera_distortion,
                        cv::noArray(),
                        cv::noArray(),
                        CV_CALIB_FIX_PRINCIPAL_POINT | CV_CALIB_SAME_FOCAL_LENGTH);
            }

            void reset() override {
                // preprocess object (checkerboard) corners
                for(int x = 0; x < EYETRIBE_CHECKERBOARD_ROWS; x++)
                    for(int y = 0; y < EYETRIBE_CHECKERBOARD_COLS; y++)
                        this->m_object_corners.push_back(EYETRIBE_CHECKERBOARD_OBJECT_COORD(x,y));

                this->m_camera_matrix.template at<float>(0) = 1750.0f;
                this->m_camera_matrix.template at<float>(1) = 0.0f;
                this->m_camera_matrix.template at<float>(2) = Format<_format>::WIDTH/2.0f;
                this->m_camera_matrix.template at<float>(3) = 0.0f;
                this->m_camera_matrix.template at<float>(4) = 3500.0f;
                this->m_camera_matrix.template at<float>(5) = Format<_format>::HEIGHT/2.0f;
                this->m_camera_matrix.template at<float>(6) = 0.0f;
                this->m_camera_matrix.template at<float>(7) = 0.0f;
                this->m_camera_matrix.template at<float>(8) = 1.0f;

                this->m_has_corners = false;
                this->m_coverage = 0.0f;
            }
        };

    }
}

#endif //EYETRIBE_CAMERACALIBRATION_H
