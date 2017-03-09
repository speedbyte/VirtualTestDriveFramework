//
// Created by geislerd on 17.02.17.
//

#ifndef CALIBRATIONREADER_H
#define CALIBRATIONREADER_H

#include <utils/Error.h>
#include <core/Node.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <opencv2/opencv.hpp>
#include <iomanip>

namespace saliency_sandbox {
    namespace kitti {
        class Calibration {
        public:
            class Cam2Cam {
            private:
                float m_corner_dist;

                cv::Matx<float, 1, 2> m_S;
                cv::Matx<float, 3, 3> m_K;
                cv::Matx<float, 1, 5> m_D;
                cv::Matx<float, 3, 3> m_R;
                cv::Matx<float, 1, 3> m_T;
                cv::Matx<float, 1, 2> m_S_rect;
                cv::Matx<float, 3, 3> m_R_rect;
                cv::Matx<float, 3, 4> m_P_rect;

                cv::Matx<float, 3, 3> m_R_rect_00;
            public:
                cv::Matx<float, 1, 2>& S();

                cv::Matx<float, 3, 3>& K();

                cv::Matx<float, 1, 5>& D();

                cv::Matx<float, 3, 3>& R();

                cv::Matx<float, 1, 3>& T();

                cv::Matx<float, 1, 2>& S_rect();

                cv::Matx<float, 3, 3>& R_rect();

                cv::Matx<float, 3, 4>& P_rect();

                cv::Matx<float, 3, 3>& R_rect_00();

                float& cornerDist();

                cv::Matx<float,4,4> TR();
            };

            class Velo2Cam {
            private:
                cv::Matx<float, 3, 3> m_R;
                cv::Matx<float, 1, 3> m_T;
                cv::Vec2f m_delta_f;
                cv::Vec2f m_delta_c;

            public:

                cv::Matx<float, 3, 3>& R();

                cv::Matx<float, 1, 3>& T();

                cv::Vec2f& deltaF();

                cv::Vec2f& deltaC();

                cv::Matx<float,4,4> TR();
            };

            class OXTS2Velo {
            private:
                cv::Matx<float, 3, 3> m_R;
                cv::Matx<float, 3, 1> m_T;

            public:

                cv::Matx<float, 3, 3>& R();

                cv::Matx<float, 3, 1>& T();
            };
        private:

            Cam2Cam m_camera[4];
            Velo2Cam m_velodyne;
            OXTS2Velo m_oxts;

            std::string read(std::ifstream& is, size_t num, float* val);

            std::string varName(std::string name, int num);


        public:

            void loadCamToCam(boost::filesystem::path path);

            void loadImuToVelo(boost::filesystem::path path);

            void loadVeloToCam(boost::filesystem::path path);

            Calibration(boost::filesystem::path path);

            void veloToCam(cv::Vec3f* velo, cv::Vec2f* cam, size_t num, size_t camera);
        };

        class CalibrationReader : public saliency_sandbox::core::Node::template Input<>::template Output<Calibration> {
        private:
            boost::filesystem::path m_path;
            Calibration m_calibration;
        public:
            CalibrationReader(boost::filesystem::path path);

            void calc() override;

            void reset() override;
        };
    }
}
#endif //CALIBRATIONREADER_H
