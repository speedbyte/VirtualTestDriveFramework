//
// Created by geislerd on 14.04.17.
//

#include <eyetribe/StereoCalibration.h>

namespace saliency_sandbox {
    namespace eyetribe {
        template<uint32_t _format>
        StereoCalibration<_format>::StereoCalibration() {
            this->template input<0>()->name("camera0 corners");
            this->template input<1>()->name("camera1 corners");
            this->template input<2>()->name("camera0 matrix");
            this->template input<3>()->name("camera0 distortion");
            this->template input<4>()->name("camera1 matrix");
            this->template input<5>()->name("camera1 distortion");

            this->template output<0>()->name("R");
            this->template output<0>()->value(&this->m_R);
            this->template output<1>()->name("T");
            this->template output<1>()->value(&this->m_T);
            this->template output<2>()->name("E");
            this->template output<2>()->value(&this->m_E);
            this->template output<3>()->name("F");
            this->template output<3>()->value(&this->m_F);
            this->template output<4>()->name("valid");
            this->template output<4>()->value(&this->m_valid);

            this->reset();
        }

        template<uint32_t _format>
        bool StereoCalibration<_format>::collectPoints() {
            std::chrono::high_resolution_clock::time_point time;
            std::chrono::duration<double, std::milli> duration;
            CameraCorners& imagePoints0 = *this->template input<0>()->value();
            CameraCorners& imagePoints1 = *this->template input<1>()->value();

            if(imagePoints0.empty() || imagePoints1.empty())
                return false; // points must be visible in both cameras

            if(imagePoints0.size() != EYETRIBE_CHECKERBOARD_NUM)
                return false;

            if(imagePoints1.size() != EYETRIBE_CHECKERBOARD_NUM)
                return false;

            time = std::chrono::high_resolution_clock::now();
            duration = time-this->m_time;

            if(duration.count() < 500.0)
                return false;

            this->m_camera_corners_list[0].push_back(imagePoints0);
            this->m_camera_corners_list[1].push_back(imagePoints1);
            this->m_object_corners_list.push_back(this->m_object_corners);

            this->m_time = std::chrono::high_resolution_clock::now();

            return this->m_object_corners_list.size() > 50;
        }

        template<uint32_t _format>
        void StereoCalibration<_format>::calc() {
            cv::Mat1d F, E, T, R;
            CameraMatrix& cameraMatrix0         = *this->template input<2>()->value();
            CameraDistortion& cameraDistortion0 = *this->template input<3>()->value();
            CameraMatrix& cameraMatrix1         = *this->template input<4>()->value();
            CameraDistortion& cameraDistortion1 = *this->template input<5>()->value();
            bool& cameraCalibrated0             = *this->template input<6>()->value();
            bool& cameraCalibrated1             = *this->template input<7>()->value();
            const cv::Size sz                   = cv::Size(Format<_format>::WIDTH,Format<_format>::HEIGHT);

            if(!cameraCalibrated0)
                return; // wait until camera 0 is calibrated

            if(!cameraCalibrated1)
                return; // wait until camera 1 is calibrated

            if(this->m_valid)
                return; // already calibrated

            if(!this->collectPoints())
                return;

            cv::stereoCalibrate(
                    this->m_object_corners_list,
                    this->m_camera_corners_list[0],
                    this->m_camera_corners_list[1],
                    cameraMatrix0.mat(),
                    cameraDistortion0.mat(),
                    cameraMatrix1.mat(),
                    cameraDistortion1.mat(),
                    sz,
                    R, T, E, F);

            R.convertTo(this->m_R,CV_32FC1);
            T.convertTo(this->m_T,CV_32FC1);
            E.convertTo(this->m_E,CV_32FC1);
            F.convertTo(this->m_F,CV_32FC1);

            this->m_valid = true;
        }

        template<uint32_t _format>
        void StereoCalibration<_format>::reset() {
            this->m_valid = false;
            this->m_R.clear();
            this->m_T.clear();
            this->m_E.clear();
            this->m_F.clear();

            this->m_object_corners_list.clear();
            this->m_camera_corners_list[0].clear();
            this->m_camera_corners_list[1].clear();

            this->m_object_corners.clear();

            // preprocess object (checkerboard) corners
            for(int x = 0; x < EYETRIBE_CHECKERBOARD_COLS; x++)
                for(int y = 0; y < EYETRIBE_CHECKERBOARD_ROWS; y++)
                    this->m_object_corners.push_back(cv::Vec3f(x*EYETRIBE_CHECKERBOARD_DISTANCE-(EYETRIBE_CHECKERBOARD_COLS-1)*EYETRIBE_CHECKERBOARD_DISTANCE*0.5f,y*EYETRIBE_CHECKERBOARD_DISTANCE-(EYETRIBE_CHECKERBOARD_ROWS-1)*EYETRIBE_CHECKERBOARD_DISTANCE*0.5f,0.0f));
        }

        template class StereoCalibration<0>;
        template class StereoCalibration<1>;
        template class StereoCalibration<2>;
        template class StereoCalibration<3>;
        template class StereoCalibration<4>;
        template class StereoCalibration<5>;
        template class StereoCalibration<6>;
        template class StereoCalibration<7>;
    }
}