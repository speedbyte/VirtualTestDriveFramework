//
// Created by geislerd on 10.02.17.
//

#ifndef VELODYNEREADER_H
#define VELODYNEREADER_H

#include <utils/Image.h>
#include <boost/filesystem/path.hpp>

namespace saliency_sandbox {
    namespace kitti {

        template<uint32_t _num_points>
        class _VelodyneMatrix : public saliency_sandbox::utils::IMatrix {
        private:
            static const size_t SIZE = sizeof(cv::Vec4f)*_num_points;
            cv::Vec4f* m_data;
            uint32_t m_height;
            bool m_eof;
        public:
            _VelodyneMatrix() : m_eof(false), m_height(0), m_data(nullptr) {
                this->m_data = (cv::Vec4f*) malloc(SIZE);
            }

            virtual ~_VelodyneMatrix() {
                delete this->m_data;
            }

            uint32_t width() override {
                return 1;
            }

            uint32_t height() override {
                return this->m_height;
            }

            void height(uint32_t height) {
                this->m_height = height;
            }

            uint32_t typeSize() override {
                return sizeof(cv::Vec4f);
            }

            uint32_t bytes() override {
                return SIZE;
            }

            void *data() override {
                return this->m_data;
            }

            cv::Mat mat() override {
                return cv::Mat4f(this->m_height,1,this->m_data);
            }

            void read(boost::filesystem::path path) {
                FILE* stream;

                this->m_height = 0;

                stream = fopen(path.c_str(),"rb");
                if(stream != nullptr)
                    this->m_height = fread(this->m_data,sizeof(cv::Vec4f),_num_points,stream);
                else
                    this->m_eof = true; //TODO: error handling - file not found or permission issue

                if(this->m_height == _num_points)
                    ; //TODO: error handling - we probably didnt get all points
            }

            bool eof() {
                return this->m_eof;
            }
        };

        template<uint32_t _num_points>
        class _ConvertVelodyne2PolarImage;

        template<uint32_t _num_points>
        class _VelodyneReader : public core::Node::template Input<>::template Output<_VelodyneMatrix<_num_points>> {
        public:
            typedef _VelodyneMatrix<_num_points> Matrix;
            typedef _ConvertVelodyne2PolarImage<_num_points> PolarImage;
        private:
            _VelodyneMatrix<_num_points> m_data;
            boost::filesystem::path m_path;

            boost::filesystem::path filename() {
                char filename[16];

                sprintf(filename,"%010ld.bin",this->time());

                return boost::filesystem::path(filename);
            }

            boost::filesystem::path filepath() {
                return this->m_path / "velodyne_points" / "data" / filename();
            }
        public:
            _VelodyneReader(boost::filesystem::path path) : m_path(path) {
                this->reset();
                //TODO: check path
            }

            void calc() override {
                this->m_data.read(this->filepath());
                this->output()->value(&(this->m_data));

                this->eof(this->m_data.eof());
            }

            void reset() override {

            }
        };

        typedef _VelodyneReader<1000000> VelodyneReader;

        typedef saliency_sandbox::utils::_HeatmapImage<360,180> VelodynePolarImage;

        template<uint32_t _num_points>
        class _ConvertVelodyne2PolarImage : public saliency_sandbox::core::Node::template Input<typename _VelodyneReader<_num_points>::Matrix>::template Output<VelodynePolarImage,VelodynePolarImage> {
        private:
            VelodynePolarImage m_distance;
            VelodynePolarImage m_reflection;

        public:
            void calc() override {
                cv::Mat4f in;
                float x,y,z,w,r;
                cv::Point c;
                cv::Vec4f* v;
                const float rad2deg = ((360.0f)/(2.0f*float(M_PI)));

                // get opencv input accessor
                in = this->template input<0>()->value()->mat();

                // clear output
                this->m_distance.setTo(0.0f);
                this->m_reflection.setTo(0.0f);

                for(int i = 0; i < in.rows; i++) {
                    v = in.ptr<cv::Vec4f>(i);
                    x = v->val[0];
                    y = v->val[1];
                    z = v->val[2];
                    w = v->val[3];
                    r = sqrtf(x*x+y*y+z*z);
                    c.y = ((int)(acosf(z/r)*rad2deg))%180;
                    c.x = ((int)((atan2f(y,x)+M_PI)*rad2deg))%360;
                    this->m_distance(c) = r;
                    this->m_reflection(c) = w;
                }

                // refresh output pointer
                this->template output<0>()->value(&(this->m_distance));
                this->template output<1>()->value(&(this->m_reflection));
            }

            void reset() override {

            }
        };
    }
}
#endif //VELODYNEREADER_H
