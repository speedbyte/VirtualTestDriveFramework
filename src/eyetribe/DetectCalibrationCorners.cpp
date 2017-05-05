//
// Created by geislerd on 05.05.17.
//

#include <eyetribe/DetectCalibrationCorners.h>


namespace saliency_sandbox {
    namespace eyetribe {

        template <uint32_t _format>
        DetectCalibrationCorners<_format>::DetectCalibrationCorners() {
            this->template input<0>()->name("image");

            this->template output<0>()->name("camera");
            this->template output<0>()->value(&this->m_camera_corners);
            this->template output<1>()->name("object");
            this->template output<1>()->value(&this->m_object_corners);
            this->template output<2>()->name("valid");
            this->template output<2>()->value(&this->m_valid);

            this->reset();
        }

        template <uint32_t _format>
        void DetectCalibrationCorners<_format>::calc() override {
            cv::Mat1b in, scaled;
            int scales, rows, cols, num;
            float dist;

            in = this->template input<0>()->value()->mat();
            scales = this->properties()->template get<int>("scales",1);
            rows = this->properties()->template get<int>("rows",8);
            cols = this->properties()->template get<int>("cols",8);
            dist = this->properties()->template get<float>("dist",12.7f);
            num = rows*cols;

            this->m_camera_corners.clear();
            this->m_object_corners.clear();

            in.copyTo(scaled);

            for(int i = 0; i < scales; i++)
                cv::pyrDown(scaled,scaled);

            this->m_valid = cv::findCirclesGrid(scaled,cv::Size(rows,cols),this->m_camera_corners);
            this->m_valid &= this->m_camera_corners.size() == num;

            if(!this->m_valid)
                return;

            for(int i = 0; i < num; i++)
                this->m_camera_corners[i] *= powf(2.0f,scales);

            for(int y = 0; y < rows; y++)
                for(int x = 0; x < cols; x++)
                    //this->m_object_corners.push_back(cv::Vec3f(x*dist-(cols-1)*dist*0.5f,y*dist-(rows-1)*dist*0.5f,0.0f));
                    this->m_object_corners.push_back(cv::Vec3f(x*dist,y*dist,0.0f));
        }

        template <uint32_t _format>
        void DetectCalibrationCorners<_format>::reset() { }
    }
}