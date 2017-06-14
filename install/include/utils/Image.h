//
// Created by geislerd on 10.02.17.
//

#ifndef IMAGE_H
#define IMAGE_H

#include <utils/Matrix.h>

namespace saliency_sandbox {
    namespace utils {

        template <uint32_t _width, uint32_t _height>
        class _RGBImage;

        template <uint32_t _width, uint32_t _height>
        class _LABImage;

        template <uint32_t _width, uint32_t _height>
        class _LMSImage;

        template <uint32_t _width, uint32_t _height>
        class _DKLImage;

        template <uint32_t _width, uint32_t _height>
        class _HSVImage;

        template <uint32_t _width, uint32_t _height>
        class _XYZImage;

        template <uint32_t _width, uint32_t _height>
        class _IntensityImage;

        template <uint32_t _width, uint32_t _height>
        class _BinaryImage;

        template <uint32_t _width, uint32_t _height>
        class _HeatmapImage;

        template <uint32_t _width, uint32_t _height, template<uint32_t,uint32_t> typename _input, template<uint32_t,uint32_t> typename _output>
        class _ImageConvert : public core::Node::template Input<_input<_width,_height>>::template Output< _output<_width,_height>> {
        public:
            typedef _input<_width,_height> InputImage;
            typedef _output<_width,_height> OutputImage;
        private:
            _output<_width,_height> m_converted;
        public:

            _ImageConvert() {
                this->template input<0>()->name("image");
                this->template output<0>()->name("output");

                this->reset();
            }

            virtual void calc() override {
                _output<_width,_height>* out;

                out = &(this->m_converted);

                this->template input<0>()->value()->convert(out);
                this->template output<0>()->value(out);
            }

            virtual void reset() override {
            };
        };

        #define define_image_converter( from , to ) \
            typedef _ImageConvert<_width,_height,_ ## from ## Image,_ ## to ## Image> Convert ## to

        template <uint32_t _width, uint32_t _height>
        class _RGBImage : public _Matrix<_width,_height,cv::Vec3b> {
        public:

            define_image_converter(RGB,LAB);
            define_image_converter(RGB,LMS);
            define_image_converter(RGB,DKL);
            define_image_converter(RGB,HSV);
            define_image_converter(RGB,XYZ);
            define_image_converter(RGB,Intensity);
            define_image_converter(RGB,Binary);
            define_image_converter(RGB,Heatmap);

            void convert(_LABImage<_width,_height>* out) {
                cv::Mat3b tmp;
                cv::cvtColor(*this,tmp,CV_RGB2Lab);
                cv::divide(tmp,cv::Scalar(100,127,127),*out,1,CV_32FC3);
                //out->mat(tmp / cv::Scalar(100,127,127));
                //cv::cvtColor(*this,*out,CV_RGB2Lab,CV_32FC3);
            }
            void convert(_LMSImage<_width,_height>* out) {
                //TODO: implement
            }
            void convert(_DKLImage<_width,_height>* out) {
                //TODO: implement
            }
            void convert(_HSVImage<_width,_height>* out) {
                cv::cvtColor(*this,*out,CV_RGB2HSV);
            }
            void convert(_XYZImage<_width,_height>* out) {
                cv::cvtColor(*this,*out,CV_RGB2XYZ);
            }
            void convert(_IntensityImage<_width,_height>* out) {
                cv::cvtColor(*this,*out,CV_RGB2GRAY);
            }
            void convert(_BinaryImage<_width,_height>* out) {
                _IntensityImage<_width,_height> intensityImage;
                this->convert(&intensityImage);
                intensityImage.convert(out);
            }
            void convert(_HeatmapImage<_width,_height>* out) {
                _IntensityImage<_width,_height> intensityImage;
                this->convert(&intensityImage);
                intensityImage.convert(out);
            }
        };

        typedef _RGBImage<RESOLUTION> RGBImage;

        template <uint32_t _width, uint32_t _height>
        class _LABImage : public _Matrix<_width,_height,cv::Vec3f> {
        public:

            define_image_converter(LAB,RGB);
            define_image_converter(LAB,LMS);
            define_image_converter(LAB,DKL);
            define_image_converter(LAB,HSV);
            define_image_converter(LAB,XYZ);
            define_image_converter(LAB,Intensity);
            define_image_converter(LAB,Binary);

            void convert(_RGBImage<_width,_height>* out) {
                cv::cvtColor(*this,*out,CV_Lab2RGB);
            }
            void convert(_LMSImage<_width,_height>* out) {
                _RGBImage<_width,_height> rgb;
                this->convert(&rgb);
                rgb.convert(out);
            }
            void convert(_DKLImage<_width,_height>* out) {
                _RGBImage<_width,_height> rgb;
                this->convert(&rgb);
                rgb.convert(out);
            }
            void convert(_HSVImage<_width,_height>* out) {
                _RGBImage<_width,_height> rgb;
                this->convert(&rgb);
                rgb.convert(out);
            }
            void convert(_XYZImage<_width,_height>* out) {
                _RGBImage<_width,_height> rgb;
                this->convert(&rgb);
                rgb.convert(out);
            }
            void convert(_IntensityImage<_width,_height>* out) {
                _RGBImage<_width,_height> rgb;
                this->convert(&rgb);
                rgb.convert(out);
            }
            void convert(_BinaryImage<_width,_height>* out) {
                _RGBImage<_width,_height> rgb;
                this->convert(&rgb);
                rgb.convert(out);
            }
            void convert(_HeatmapImage<_width,_height>* out) {
                _IntensityImage<_width,_height> intensityImage;
                this->convert(&intensityImage);
                intensityImage.convert(out);
            }
        };

        typedef _LABImage<RESOLUTION> LABImage;

        template <uint32_t _width, uint32_t _height>
        class _LMSImage : public _Matrix<_width,_height,cv::Vec3f> {
        public:

            define_image_converter(LMS,RGB);
            define_image_converter(LMS,LAB);
            define_image_converter(LMS,DKL);
            define_image_converter(LMS,HSV);
            define_image_converter(LMS,XYZ);
            define_image_converter(LMS,Intensity);
            define_image_converter(LMS,Binary);
            define_image_converter(LMS,Heatmap);

            void convert(_RGBImage<_width,_height>* out) {
                //TODO: implement
            }
            void convert(_LABImage<_width,_height>* out) {
                _RGBImage<_width,_height> rgb;
                this->convert(&rgb);
                rgb.convert(out);
            }
            void convert(_DKLImage<_width,_height>* out) {
                _RGBImage<_width,_height> rgb;
                this->convert(&rgb);
                rgb.convert(out);
            }
            void convert(_HSVImage<_width,_height>* out) {
                _RGBImage<_width,_height> rgb;
                this->convert(&rgb);
                rgb.convert(out);
            }
            void convert(_XYZImage<_width,_height>* out) {
                _RGBImage<_width,_height> rgb;
                this->convert(&rgb);
                rgb.convert(out);
            }
            void convert(_IntensityImage<_width,_height>* out) {
                _RGBImage<_width,_height> rgb;
                this->convert(&rgb);
                rgb.convert(out);
            }
            void convert(_BinaryImage<_width,_height>* out) {
                _RGBImage<_width,_height> rgb;
                this->convert(&rgb);
                rgb.convert(out);
            }
            void convert(_HeatmapImage<_width,_height>* out) {
                _IntensityImage<_width,_height> intensityImage;
                this->convert(&intensityImage);
                intensityImage.convert(out);
            }
        };

        typedef _LMSImage<RESOLUTION> LMSImage;

        template <uint32_t _width, uint32_t _height>
        class _DKLImage : public _Matrix<_width,_height,cv::Vec3f> {
        public:
            define_image_converter(DKL,RGB);
            define_image_converter(DKL,LAB);
            define_image_converter(DKL,LMS);
            define_image_converter(DKL,HSV);
            define_image_converter(DKL,XYZ);
            define_image_converter(DKL,Intensity);
            define_image_converter(DKL,Binary);
            define_image_converter(DKL,Heatmap);

            void convert(_RGBImage<_width,_height>* out) {
                //TODO: implement
            }
            void convert(_LABImage<_width,_height>* out) {
                _RGBImage<_width,_height> rgb;
                this->convert(&rgb);
                rgb.convert(out);
            }
            void convert(_LMSImage<_width,_height>* out) {
                _RGBImage<_width,_height> rgb;
                this->convert(&rgb);
                rgb.convert(out);
            }
            void convert(_HSVImage<_width,_height>* out) {
                _RGBImage<_width,_height> rgb;
                this->convert(&rgb);
                rgb.convert(out);
            }
            void convert(_XYZImage<_width,_height>* out) {
                _RGBImage<_width,_height> rgb;
                this->convert(&rgb);
                rgb.convert(out);
            }
            void convert(_IntensityImage<_width,_height>* out) {
                _RGBImage<_width,_height> rgb;
                this->convert(&rgb);
                rgb.convert(out);
            }
            void convert(_BinaryImage<_width,_height>* out) {
                _RGBImage<_width,_height> rgb;
                this->convert(&rgb);
                rgb.convert(out);
            }
            void convert(_HeatmapImage<_width,_height>* out) {
                _IntensityImage<_width,_height> intensityImage;
                this->convert(&intensityImage);
                intensityImage.convert(out);
            }
        };

        typedef _DKLImage<RESOLUTION> DKLImage;

        template <uint32_t _width, uint32_t _height>
        class _HSVImage : public _Matrix<_width,_height,cv::Vec3f> {
        public:

            define_image_converter(HSV,RGB);
            define_image_converter(HSV,LAB);
            define_image_converter(HSV,LMS);
            define_image_converter(HSV,DKL);
            define_image_converter(HSV,XYZ);
            define_image_converter(HSV,Intensity);
            define_image_converter(HSV,Binary);
            define_image_converter(HSV,Heatmap);

            void convert(_RGBImage<_width,_height>* out) {
                cv::Mat3f rgb;
                cv::cvtColor(*this,rgb,cv::COLOR_HSV2RGB);
                rgb.convertTo(*out,CV_8UC3,255);
            }
            void convert(_LABImage<_width,_height>* out) {
                _RGBImage<_width,_height> rgb;
                this->convert(&rgb);
                rgb.convert(out);
            }
            void convert(_LMSImage<_width,_height>* out) {
                _RGBImage<_width,_height> rgb;
                this->convert(&rgb);
                rgb.convert(out);
            }
            void convert(_DKLImage<_width,_height>* out) {
                _RGBImage<_width,_height> rgb;
                this->convert(&rgb);
                rgb.convert(out);
            }
            void convert(_XYZImage<_width,_height>* out) {
                _RGBImage<_width,_height> rgb;
                this->convert(&rgb);
                rgb.convert(out);
            }
            void convert(_IntensityImage<_width,_height>* out) {
                _RGBImage<_width,_height> rgb;
                this->convert(&rgb);
                rgb.convert(out);
            }
            void convert(_BinaryImage<_width,_height>* out) {
                _RGBImage<_width,_height> rgb;
                this->convert(&rgb);
                rgb.convert(out);
            }
            void convert(_HeatmapImage<_width,_height>* out) {
                _IntensityImage<_width,_height> intensityImage;
                this->convert(&intensityImage);
                intensityImage.convert(out);
            }
        };

        typedef _HSVImage<RESOLUTION> HSVImage;

        template <uint32_t _width, uint32_t _height>
        class _XYZImage : public _Matrix<_width,_height,cv::Vec3f> {
        public:

            define_image_converter(XYZ,RGB);
            define_image_converter(XYZ,LAB);
            define_image_converter(XYZ,LMS);
            define_image_converter(XYZ,DKL);
            define_image_converter(XYZ,HSV);
            define_image_converter(XYZ,Intensity);
            define_image_converter(XYZ,Binary);
            define_image_converter(XYZ,Heatmap);

            void convert(_RGBImage<_width,_height>* out) {
                cv::cvtColor(*this,*out,CV_XYZ2RGB);
            }
            void convert(_LABImage<_width,_height>* out) {
                _RGBImage<_width,_height> rgb;
                this->convert(&rgb);
                rgb.convert(out);
            }
            void convert(_LMSImage<_width,_height>* out) {
                _RGBImage<_width,_height> rgb;
                this->convert(&rgb);
                rgb.convert(out);
            }
            void convert(_DKLImage<_width,_height>* out) {
                _RGBImage<_width,_height> rgb;
                this->convert(&rgb);
                rgb.convert(out);
            }
            void convert(_HSVImage<_width,_height>* out) {
                _RGBImage<_width,_height> rgb;
                this->convert(&rgb);
                rgb.convert(out);
            }
            void convert(_IntensityImage<_width,_height>* out) {
                _RGBImage<_width,_height> rgb;
                this->convert(&rgb);
                rgb.convert(out);
            }
            void convert(_BinaryImage<_width,_height>* out) {
                _RGBImage<_width,_height> rgb;
                this->convert(&rgb);
                rgb.convert(out);
            }
            void convert(_HeatmapImage<_width,_height>* out) {
                _IntensityImage<_width,_height> intensityImage;
                this->convert(&intensityImage);
                intensityImage.convert(out);
            }
        };

        typedef _XYZImage<RESOLUTION> XYZImage;

        template <uint32_t _width, uint32_t _height>
        class _IntensityImage : public _Matrix<_width,_height,uchar> {
        public:

            define_image_converter(Intensity,RGB);
            define_image_converter(Intensity,LAB);
            define_image_converter(Intensity,LMS);
            define_image_converter(Intensity,DKL);
            define_image_converter(Intensity,HSV);
            define_image_converter(Intensity,XYZ);
            define_image_converter(Intensity,Binary);
            define_image_converter(Intensity,Heatmap);

            void convert(_RGBImage<_width,_height>* out) {
                cv::cvtColor(*this,*out,CV_GRAY2RGB);
            }
            void convert(_LABImage<_width,_height>* out) {
                _RGBImage<_width,_height> rgb;
                this->convert(&rgb);
                rgb.convert(out);
            }
            void convert(_LMSImage<_width,_height>* out) {
                _RGBImage<_width,_height> rgb;
                this->convert(&rgb);
                rgb.convert(out);
            }
            void convert(_DKLImage<_width,_height>* out) {
                _RGBImage<_width,_height> rgb;
                this->convert(&rgb);
                rgb.convert(out);
            }
            void convert(_HSVImage<_width,_height>* out) {
                _RGBImage<_width,_height> rgb;
                this->convert(&rgb);
                rgb.convert(out);
            }
            void convert(_XYZImage<_width,_height>* out) {
                _RGBImage<_width,_height> rgb;
                this->convert(&rgb);
                rgb.convert(out);
            }
            void convert(_BinaryImage<_width,_height>* out) {
                cv::threshold(*this,*out,128,255,CV_8UC1);
            }
            void convert(_HeatmapImage<_width,_height>* out) {
                cv::divide(*this,255.0f,*out,1,CV_32FC1);
            }
        };

        typedef _IntensityImage<RESOLUTION> IntensityImage;

        template <uint32_t _width, uint32_t _height>
        class _BinaryImage : public _Matrix<_width,_height,uchar> {
        public:

            define_image_converter(Binary,RGB);
            define_image_converter(Binary,LAB);
            define_image_converter(Binary,LMS);
            define_image_converter(Binary,DKL);
            define_image_converter(Binary,HSV);
            define_image_converter(Binary,XYZ);
            define_image_converter(Binary,Intensity);
            define_image_converter(Binary,Heatmap);

            void convert(_RGBImage<_width,_height>* out) {
                //TODO: implement
            }
            void convert(_LABImage<_width,_height>* out) {
                _RGBImage<_width,_height> rgb;
                this->convert(&rgb);
                rgb.convert(out);
            }
            void convert(_LMSImage<_width,_height>* out) {
                _RGBImage<_width,_height> rgb;
                this->convert(&rgb);
                rgb.convert(out);
            }
            void convert(_DKLImage<_width,_height>* out) {
                _RGBImage<_width,_height> rgb;
                this->convert(&rgb);
                rgb.convert(out);
            }
            void convert(_HSVImage<_width,_height>* out) {
                _RGBImage<_width,_height> rgb;
                this->convert(&rgb);
                rgb.convert(out);
            }
            void convert(_XYZImage<_width,_height>* out) {
                _RGBImage<_width,_height> rgb;
                this->convert(&rgb);
                rgb.convert(out);
            }
            void convert(_IntensityImage<_width,_height>* out) {
                _RGBImage<_width,_height> rgb;
                this->convert(&rgb);
                rgb.convert(out);
            }
            void convert(_HeatmapImage<_width,_height>* out) {
                _IntensityImage<_width,_height> intensityImage;
                this->convert(&intensityImage);
                intensityImage.convert(out);
            }
        };

        typedef _BinaryImage<RESOLUTION> BinaryImage;

        template <uint32_t _width, uint32_t _height>
        class _HeatmapImage : public _Matrix<_width,_height,float> {
        public:

            define_image_converter(Heatmap,RGB);
            define_image_converter(Heatmap,LAB);
            define_image_converter(Heatmap,LMS);
            define_image_converter(Heatmap,DKL);
            define_image_converter(Heatmap,HSV);
            define_image_converter(Heatmap,XYZ);
            define_image_converter(Heatmap,Intensity);
            define_image_converter(Heatmap,Binary);

            void convert(_RGBImage<_width,_height>* out) {
                _IntensityImage<_width,_height> intensity;
                this->convert(&intensity);
                cv::applyColorMap(intensity.mat(),out->mat(),cv::COLORMAP_JET);
            }
            void convert(_LABImage<_width,_height>* out) {
                _RGBImage<_width,_height> rgb;
                this->convert(&rgb);
                rgb.convert(out);
            }
            void convert(_LMSImage<_width,_height>* out) {
                _RGBImage<_width,_height> rgb;
                this->convert(&rgb);
                rgb.convert(out);
            }
            void convert(_DKLImage<_width,_height>* out) {
                _RGBImage<_width,_height> rgb;
                this->convert(&rgb);
                rgb.convert(out);
            }
            void convert(_HSVImage<_width,_height>* out) {
                _RGBImage<_width,_height> rgb;
                this->convert(&rgb);
                rgb.convert(out);
            }
            void convert(_XYZImage<_width,_height>* out) {
                _RGBImage<_width,_height> rgb;
                this->convert(&rgb);
                rgb.convert(out);
            }
            void convert(_IntensityImage<_width,_height>* out) {
                cv::normalize(*this,*out,0,255,cv::NORM_MINMAX,CV_8UC1);
            }
            void convert(_BinaryImage<_width,_height>* out) {
                _IntensityImage<_width,_height> intensity;
                this->convert(&intensity);
                intensity.convert(out);
            }
        };

        typedef _HeatmapImage<RESOLUTION> HeatmapImage;
    }
}

#endif //IMAGE_H
