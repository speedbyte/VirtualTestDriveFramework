//
// Created by geislerd on 18.04.17.
//

#ifndef EYETRIBE_DETECTCALIBRATIONCORNERS_H
#define EYETRIBE_DETECTCALIBRATIONCORNERS_H

#include <eyetribe/VideoReader.h>

namespace saliency_sandbox {
    namespace eyetribe {
        typedef std::vector<cv::Vec2f> CameraCorners;
        typedef std::vector<cv::Vec3f> ObjectCorners;

        template <uint32_t _format>
        class DetectCalibrationCorners : public saliency_sandbox::core::Node::
        template Input<typename VideoReader<_format>::Image>::
        template Output<
                CameraCorners, // detected corners
                ObjectCorners, // actual corners
                bool           // valid
        > {
        private:
            CameraCorners m_camera_corners;
            ObjectCorners m_object_corners;
            bool m_valid;

        public:
            DetectCalibrationCorners() {
                this->template input<0>()->name("image");

                this->template output<0>()->name("camera");
                this->template output<0>()->value(&this->m_camera_corners);
                this->template output<1>()->name("object");
                this->template output<1>()->value(&this->m_object_corners);
                this->template output<2>()->name("valid");
                this->template output<2>()->value(&this->m_valid);

                this->reset();
            }

            void calc() override {
                cv::Mat1b in, scaled;
                int scales, rows, cols, num;
                float dist;

                in = this->template input<0>()->value()->mat();
                scales = this->properties()->template get<int>("scales",1);
                rows = this->properties()->template get<int>("rows",4);
                cols = this->properties()->template get<int>("cols",4);
                dist = this->properties()->template get<float>("dist",14);
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

                for(int x = 0; x < cols; x++)
                    for(int y = 0; y < rows; y++)
                        this->m_object_corners.push_back(cv::Vec3f(x*dist-(cols-1)*dist*0.5f,y*dist-(rows-1)*dist*0.5f,0.0f));
            }

            void reset() override { }
        };
    }
}
#endif //EYETRIBE_DETECTCALIBRATIONCORNERS_H
