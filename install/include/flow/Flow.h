//
// Created by geislerd on 15.03.17.
//

#include <utils/Image.h>

#ifndef FLOW_H
#define FLOW_H
namespace saliency_sandbox {
    namespace flow {

        template<uint32_t _width, uint32_t _height>
        class _FlowField : public saliency_sandbox::utils::_Matrix<_width, _height, cv::Vec2f> {
        public:
            typedef cv::Vec2f FlowVec;

            typedef saliency_sandbox::utils::_ImageConvert<_width,_height,_FlowField,saliency_sandbox::utils::_RGBImage> ConvertRGB;
            typedef saliency_sandbox::utils::_ImageConvert<_width,_height,_FlowField,saliency_sandbox::utils::_LMSImage> ConvertLMS;
            typedef saliency_sandbox::utils::_ImageConvert<_width,_height,_FlowField,saliency_sandbox::utils::_LABImage> ConvertLAB;
            typedef saliency_sandbox::utils::_ImageConvert<_width,_height,_FlowField,saliency_sandbox::utils::_DKLImage> ConvertDKL;
            typedef saliency_sandbox::utils::_ImageConvert<_width,_height,_FlowField,saliency_sandbox::utils::_HSVImage> ConvertHSV;
            typedef saliency_sandbox::utils::_ImageConvert<_width,_height,_FlowField,saliency_sandbox::utils::_XYZImage> ConvertXYZ;
            typedef saliency_sandbox::utils::_ImageConvert<_width,_height,_FlowField,saliency_sandbox::utils::_IntensityImage> ConvertIntensity;
            typedef saliency_sandbox::utils::_ImageConvert<_width,_height,_FlowField,saliency_sandbox::utils::_BinaryImage> ConvertBinary;

            void convert(saliency_sandbox::utils::_RGBImage<_width,_height>* out) {
                saliency_sandbox::utils::_HSVImage<_width,_height> hsv;
                this->convert(&hsv);
                hsv.convert(out);
            }
            void convert(saliency_sandbox::utils::_LMSImage<_width,_height>* out) {
                saliency_sandbox::utils::_HSVImage<_width,_height> hsv;
                this->convert(&hsv);
                hsv.convert(out);
            }
            void convert(saliency_sandbox::utils::_LABImage<_width,_height>* out) {
                saliency_sandbox::utils::_HSVImage<_width,_height> hsv;
                this->convert(&hsv);
                hsv.convert(out);
            }
            void convert(saliency_sandbox::utils::_DKLImage<_width,_height>* out) {
                saliency_sandbox::utils::_HSVImage<_width,_height> hsv;
                this->convert(&hsv);
                hsv.convert(out);
            }
            void convert(saliency_sandbox::utils::_HSVImage<_width,_height>* out) {
                cv::Mat1f xy[2], magnitude, angle, hsvc[3];
                cv::Scalar max;

                cv::split(this->mat(),xy);
                cv::cartToPolar(xy[0],xy[1],magnitude,angle,true);
                cv::minMaxLoc(magnitude, nullptr,max.val);
                cv::divide(magnitude,max,magnitude);
                hsvc[0] = angle;
                hsvc[1] = cv::Mat1f::ones(_height,_width);
                hsvc[2] = magnitude;
                cv::merge(hsvc,3,*out);
            }
            void convert(saliency_sandbox::utils::_XYZImage<_width,_height>* out) {
                saliency_sandbox::utils::_HSVImage<_width,_height> hsv;
                this->convert(&hsv);
                hsv.convert(out);
            }
            void convert(saliency_sandbox::utils::_IntensityImage<_width,_height>* out) {
                saliency_sandbox::utils::_HSVImage<_width,_height> hsv;
                this->convert(&hsv);
                hsv.convert(out);
            }
            void convert(saliency_sandbox::utils::_BinaryImage<_width,_height>* out) {
                saliency_sandbox::utils::_HSVImage<_width,_height> hsv;
                this->convert(&hsv);
                hsv.convert(out);
            }
            void convert(saliency_sandbox::utils::_HeatmapImage<_width,_height>* out) {
                saliency_sandbox::utils::_IntensityImage<_width,_height> intensityImage;
                this->convert(&intensityImage);
                intensityImage.convert(out);
            }
        };

        template<uint32_t _width, uint32_t _height, typename... _input>
        class _Flow : public core::Node::template Input<_input...>::template Output<_FlowField<_width,_height>> {
        public:
            typedef _FlowField<_width, _height> FlowField;

        private:
            FlowField m_flow;

        public:
            _Flow() {
                this->template output<0>()->name("flow");
                this->template output<0>()->value(&(this->m_flow));
            };

        protected:

            typename FlowField::Mat flow() {
                    return this->m_flow.mat();
            }

            void flow(typename FlowField::Mat::value_type v) {
                typename FlowField::Mat m;

                m = this->flow();
                for(cv::Point xy(0,0); xy.x < m.cols; xy.x++)
                    for(xy.y = 0; xy.y < m.rows; xy.y++)
                        m(xy) = v;
            }
        };
    }
}
#endif //FLOW_H
