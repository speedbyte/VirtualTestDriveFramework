//
// Created by geislerd on 07.02.17.
//

#ifndef MATRIX_H
#define MATRIX_H

#include <config.h>
#include <opencv2/opencv.hpp>
#include <core/Node.h>

namespace saliency_sandbox {
    namespace utils {

        template <uint32_t _width, uint32_t _height, typename _type>
        class _MatrixSplitt;

        template <uint32_t _width, uint32_t _height, typename _type>
        class _MatrixMerge;

        template <uint32_t _width, uint32_t _height, typename _type>
        class _MatrixSum;

        class IMatrix {
        public:
            virtual uint32_t width() = 0;
            virtual uint32_t height() = 0;
            virtual uint32_t typeSize() = 0;
            virtual uint32_t bytes() = 0;
            virtual void* data() = 0;
            virtual cv::Mat mat() = 0;
        };

        template <uint32_t _width, uint32_t _height, typename _type>
        class _Matrix : public cv::Mat_<_type>, public IMatrix {
        public:
            const static size_t WIDTH = _width;
            const static size_t HEIGHT = _height;
            const static size_t TYPE_SIZE = sizeof(_type);
            const static size_t BYTES = WIDTH*HEIGHT*sizeof(_type);

            typedef cv::Mat_<_type> Mat;
            typedef _type ValueType;
            typedef _MatrixSplitt<_width,_height,_type> Splitt;
            typedef _MatrixMerge<_width,_height,_type> Merge;
            typedef _MatrixSum<_width,_height,_type> Sum;
        private:
            _type m_value[_height*_width];
        public:
            _Matrix() : cv::Mat_<_type>(_height,_width,this->m_value) {

            }

            cv::Mat mat() override {
                return cv::Mat_<_type>(_height,_width,this->m_value);
            }

            void mat(const cv::Mat& mat) {
                memcpy(this->m_value,mat.data,this->bytes());
            }

            uint32_t width() override {
                return _width;
            }

            uint32_t height() override {
                return _height;
            }

            uint32_t typeSize() override {
                return sizeof(_type);
            }

            uint32_t bytes() override {
                return _width*_height*sizeof(_type);
            }

            void* data() override {
                return this->m_value;
            }

            void clear() {
                memset(this->m_value,0,this->bytes());
            }
        };

        template <uint32_t _width, uint32_t _height, typename _type>
        class _MatrixSplitt : public core::Node::template Input<_Matrix<_width,_height,_type>>::template Output<_Matrix<_width,_height,_type>> {
        public:
            _MatrixSplitt() {
                this->reset();

                this->template input<0>()->name("input");
                this->template output<0>()->name("channel 0");
            }

            void calc() override {

            }

            void reset() override {
                this->output()->connect(this->input());
            }
        };

        template <uint32_t _width, uint32_t _height>
        class _MatrixSplitt<_width,_height,cv::Vec2f> : public core::Node::template Input<_Matrix<_width,_height,cv::Vec2f>>::template Output<_Matrix<_width,_height,float>,_Matrix<_width,_height,float>> {
        private:
            _Matrix<_width,_height,float> m_channel[2];
            cv::Mat1f m_channel_mat[2] = {m_channel[0].mat(),m_channel[1].mat()};
        public:
            _MatrixSplitt() {
                this->reset();

                this->template input<0>()->name("input");
                this->template output<1>()->name("channel 0");
                this->template output<2>()->name("channel 1");
            }

            void calc() override {
                cv::split(*(this->input()->value()),this->m_channel_mat);

                this->template output<0>()->value(&(this->m_channel[0]));
                this->template output<1>()->value(&(this->m_channel[1]));
            }

            void reset() override {
            }
        };

        template <uint32_t _width, uint32_t _height>
        class _MatrixSplitt<_width,_height,cv::Vec3f> : public core::Node::template Input<_Matrix<_width,_height,cv::Vec3f>>::template Output<_Matrix<_width,_height,float>,_Matrix<_width,_height,float>,_Matrix<_width,_height,float>> {
        private:
            _Matrix<_width,_height,float> m_channel[3];
            cv::Mat1f m_channel_mat[3] = {m_channel[0].mat(),m_channel[1].mat(),m_channel[2].mat()};
        public:
            _MatrixSplitt() {
                this->reset();

                this->template input<0>()->name("input");
                this->template output<0>()->name("channel 0");
                this->template output<1>()->name("channel 1");
                this->template output<2>()->name("channel 2");
            }

            void calc() override {
                cv::split(*(this->input()->value()),this->m_channel_mat);

                this->template output<0>()->value(&(this->m_channel[0]));
                this->template output<1>()->value(&(this->m_channel[1]));
                this->template output<2>()->value(&(this->m_channel[2]));
            }

            void reset() override {
            }
        };

        template <uint32_t _width, uint32_t _height>
        class _MatrixSplitt<_width,_height,cv::Vec3b> : public core::Node::template Input<_Matrix<_width,_height,cv::Vec3b>>::template Output<_Matrix<_width,_height,uchar>,_Matrix<_width,_height,uchar>,_Matrix<_width,_height,uchar>> {
        private:
            _Matrix<_width,_height,uchar> m_channel[3];
            cv::Mat1b m_channel_mat[3] = {m_channel[0].mat(),m_channel[1].mat(),m_channel[2].mat()};
        public:
            _MatrixSplitt() {
                this->reset();

                this->template input<0>()->name("input");
                this->template output<0>()->name("channel 0");
                this->template output<1>()->name("channel 1");
                this->template output<2>()->name("channel 2");
            }

            void calc() override {
                cv::split(*(this->input()->value()),this->m_channel_mat);

                this->template output<0>()->value(&(this->m_channel[0]));
                this->template output<1>()->value(&(this->m_channel[1]));
                this->template output<2>()->value(&(this->m_channel[2]));
            }

            void reset() override {
            }
        };

        template <uint32_t _width, uint32_t _height, typename _type>
        class _MatrixSum : public core::Node::template Input<_Matrix<_width,_height,_type>>::template Output<_Matrix<_width,_height,_type>> {
        public:
            _MatrixSum() {
                this->reset();

                this->template input<0>()->name("input");
                this->template output<0>()->name("sum");
            }

            void calc() override {

            }

            void reset() override {
                this->output()->connect(this->input());
            }
        };

        template <uint32_t _width, uint32_t _height>
        class _MatrixSum<_width,_height, cv::Vec2f> : public core::Node::template Input<_Matrix<_width,_height,cv::Vec2f>>::template Output<_Matrix<_width,_height,float>> {
        private:
            _Matrix<_width,_height,float> m_sum;
        public:
            _MatrixSum() {
                this->reset();

                this->template input<0>()->name("input");
                this->template output<0>()->name("sum");
            }

            void calc() override {
                cv::transform(*this->input()->value(),this->m_sum,cv::Matx12f(1,1));
                this->output()->value(&this->m_sum);
            }

            void reset() override {
                this->output()->value(&this->m_sum);
            }
        };

        template <uint32_t _width, uint32_t _height>
        class _MatrixSum<_width,_height, cv::Vec3f> : public core::Node::template Input<_Matrix<_width,_height,cv::Vec3f>>::template Output<_Matrix<_width,_height,float>> {
        private:
            _Matrix<_width,_height,float> m_sum;
        public:
            _MatrixSum() {
                this->reset();

                this->template input<0>()->name("input");
                this->template output<0>()->name("sum");
            }

            void calc() override {
                cv::transform(*this->input()->value(),this->m_sum,cv::Matx13f(1,1,1));
                this->output()->value(&this->m_sum);
            }

            void reset() override {
                this->output()->value(&this->m_sum);
            }
        };

        template <uint32_t _width, uint32_t _height>
        class _MatrixSum<_width,_height, cv::Vec4f> : public core::Node::template Input<_Matrix<_width,_height,cv::Vec4f>>::template Output<_Matrix<_width,_height,float>> {
        private:
            _Matrix<_width,_height,float> m_sum;
        public:
            _MatrixSum() {
                this->reset();

                this->template input<0>()->name("input");
                this->template output<0>()->name("sum");
            }

            void calc() override {
                cv::transform(*this->input()->value(),this->m_sum,cv::Matx14f(1,1,1,1));
                this->output()->value(&this->m_sum);
            }

            void reset() override {
                this->output()->value(&this->m_sum);
            }
        };

        template <uint32_t _width, uint32_t _height>
        class _MatrixSum<_width,_height, cv::Vec<float, 5>> : public core::Node::template Input<_Matrix<_width,_height,cv::Vec<float, 5>>>::template Output<_Matrix<_width,_height,float>> {
        private:
            _Matrix<_width,_height,float> m_sum;
        public:
            _MatrixSum() {
                this->reset();

                this->template input<0>()->name("input");
                this->template output<0>()->name("sum");
            }

            void calc() override {
                cv::transform(*this->input()->value(),this->m_sum,cv::Matx<float, 1, 5>(1,1,1,1,1));
                this->output()->value(&this->m_sum);
            }

            void reset() override {
                this->output()->value(&this->m_sum);
            }
        };

        template <uint32_t _width, uint32_t _height>
        class _MatrixSum<_width,_height, cv::Vec3b> : public core::Node::template Input<_Matrix<_width,_height,cv::Vec3b>>::template Output<_Matrix<_width,_height,uchar>> {
        private:
            _Matrix<_width,_height,uchar> m_sum;
        public:
            _MatrixSum() {
                this->reset();

                this->template input<0>()->name("input");
                this->template output<0>()->name("sum");
            }

            void calc() override {
                cv::transform(*this->input()->value(),this->m_sum,cv::Matx13f(1,1,1));
                this->output()->value(&this->m_sum);
            }

            void reset() override {
                this->output()->value(&this->m_sum);
            }
        };

        template <uint32_t _width, uint32_t _height, typename _type>
        class _MatrixMerge : public core::Node::template Input<_Matrix<_width,_height,_type>>::template Output<_Matrix<_width,_height,_type>> {
        public:
            _MatrixMerge() {
                this->reset();

                this->template input<0>()->name("channel 0");
                this->template output<0>()->name("merged");
            }

            void calc() override {

            }

            void reset() override {
                this->output()->connect(this->input());
            }
        };

        template <uint32_t _width, uint32_t _height>
        class _MatrixMerge<_width,_height,cv::Vec2f> : public core::Node::template Input<_Matrix<_width,_height,float>,_Matrix<_width,_height,float>>::template Output<_Matrix<_width,_height,cv::Vec2f>> {
        private:
            _Matrix<_width,_height,cv::Vec2f> m_merged;
        public:
            _MatrixMerge() {
                this->reset();

                this->template input<0>()->name("channel 0");
                this->template input<1>()->name("channel 1");
                this->template output<0>()->name("merged");
            }

            void calc() override {
                cv::Mat1f m_channel_mat[2] = {this->template input<0>()->value()->mat(),this->template input<1>()->value()->mat()};

                cv::merge(m_channel_mat,2,this->m_merged);
                this->template output<0>()->value(&(this->m_merged));
            }

            void reset() override {
            }
        };

        template <uint32_t _width, uint32_t _height>
        class _MatrixMerge<_width,_height,cv::Vec3f> : public core::Node::template Input<_Matrix<_width,_height,float>,_Matrix<_width,_height,float>,_Matrix<_width,_height,float>>::template Output<_Matrix<_width,_height,cv::Vec3f>> {
        private:
            _Matrix<_width,_height,cv::Vec3f> m_merged;
        public:
            _MatrixMerge() {
                this->reset();

                this->template input<0>()->name("channel 0");
                this->template input<1>()->name("channel 1");
                this->template input<2>()->name("channel 2");
                this->template output<0>()->name("merged");
            }

            void calc() override {
                cv::Mat1f m_channel_mat[3] = {this->template input<0>()->value()->mat(),this->template input<1>()->value()->mat(),this->template input<2>()->value()->mat()};

                cv::merge(m_channel_mat,3,this->m_merged);
                this->template output<0>()->value(&(this->m_merged));
            }

            void reset() override {
            }
        };

        template <uint32_t _width, uint32_t _height>
        class _MatrixMerge<_width,_height,cv::Vec4f> : public core::Node::template Input<_Matrix<_width,_height,float>,_Matrix<_width,_height,float>,_Matrix<_width,_height,float>,_Matrix<_width,_height,float>>::template Output<_Matrix<_width,_height,cv::Vec4f>> {
        private:
            _Matrix<_width,_height,cv::Vec4f> m_merged;
        public:
            _MatrixMerge() {
                this->reset();

                this->template input<0>()->name("channel 0");
                this->template input<1>()->name("channel 1");
                this->template input<2>()->name("channel 2");
                this->template input<3>()->name("channel 4");
                this->template output<0>()->name("merged");
            }

            void calc() override {
                cv::Mat1f m_channel_mat[4] = {
                        this->template input<0>()->value()->mat(),
                        this->template input<1>()->value()->mat(),
                        this->template input<2>()->value()->mat(),
                        this->template input<3>()->value()->mat()};

                cv::merge(m_channel_mat,4,this->m_merged);
                this->template output<0>()->value(&(this->m_merged));
            }

            void reset() override {
            }
        };

        template <uint32_t _width, uint32_t _height>
        class _MatrixMerge<_width,_height,cv::Vec<float,5>> : public core::Node::template Input<_Matrix<_width,_height,float>,_Matrix<_width,_height,float>,_Matrix<_width,_height,float>,_Matrix<_width,_height,float>,_Matrix<_width,_height,float>>::template Output<_Matrix<_width,_height,cv::Vec<float,5>>> {
        private:
            _Matrix<_width,_height,cv::Vec<float,5>> m_merged;
        public:
            _MatrixMerge() {
                this->reset();

                this->template input<0>()->name("channel 0");
                this->template input<1>()->name("channel 1");
                this->template input<2>()->name("channel 2");
                this->template input<3>()->name("channel 4");
                this->template input<4>()->name("channel 5");
                this->template output<0>()->name("merged");
            }

            void calc() override {
                cv::Mat1f m_channel_mat[5] = {
                        this->template input<0>()->value()->mat(),
                        this->template input<1>()->value()->mat(),
                        this->template input<2>()->value()->mat(),
                        this->template input<3>()->value()->mat(),
                        this->template input<4>()->value()->mat()};

                cv::merge(m_channel_mat,5,this->m_merged);
                this->template output<0>()->value(&(this->m_merged));
            }

            void reset() override {
            }
        };

        template <uint32_t _width, uint32_t _height>
        class _MatrixMerge<_width,_height,cv::Vec3b> : public core::Node::template Input<_Matrix<_width,_height,uchar>,_Matrix<_width,_height,uchar>,_Matrix<_width,_height,uchar>>::template Output<_Matrix<_width,_height,cv::Vec3b>> {
        private:
            _Matrix<_width,_height,cv::Vec3b> m_merged;
        public:
            _MatrixMerge() {
                this->reset();

                this->template input<0>()->name("channel 0");
                this->template input<1>()->name("channel 1");
                this->template input<2>()->name("channel 2");
                this->template output<0>()->name("merged");
            }

            void calc() override {
                cv::Mat1b m_channel_mat[3] = {this->template input<0>()->value()->mat(),this->template input<1>()->value()->mat(),this->template input<2>()->value()->mat()};

                cv::merge(m_channel_mat,3,this->m_merged);
                this->template output<0>()->value(&(this->m_merged));
            }

            void reset() override {
            }
        };

    }
}
#endif //MATRIX_H
