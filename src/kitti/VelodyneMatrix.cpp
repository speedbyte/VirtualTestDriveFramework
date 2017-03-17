//
// Created by geislerd on 16.03.17.
//

#include <kitti/VelodyneReader.h>

namespace saliency_sandbox {
    namespace kitti {
        template<uint32_t _num_points>
        _VelodyneMatrix<_num_points>::_VelodyneMatrix() : m_eof(false), m_height(0), m_data(nullptr) {
            this->m_data = (cv::Vec4f*) malloc(SIZE);
        }

        template<uint32_t _num_points>
        _VelodyneMatrix<_num_points>::~_VelodyneMatrix() {
            delete this->m_data;
        }

        template<uint32_t _num_points>
        uint32_t _VelodyneMatrix<_num_points>::width() {
            return 1;
        }

        template<uint32_t _num_points>
        uint32_t _VelodyneMatrix<_num_points>::height() {
            return this->m_height;
        }

        template<uint32_t _num_points>
        void _VelodyneMatrix<_num_points>::height(uint32_t height) {
            this->m_height = height;
        }

        template<uint32_t _num_points>
        uint32_t _VelodyneMatrix<_num_points>::typeSize() {
            return sizeof(cv::Vec4f);
        }

        template<uint32_t _num_points>
        uint32_t _VelodyneMatrix<_num_points>::bytes() {
            return SIZE;
        }

        template<uint32_t _num_points>
        void* _VelodyneMatrix<_num_points>::data() {
            return this->m_data;
        }

        template<uint32_t _num_points>
        cv::Mat _VelodyneMatrix<_num_points>::mat() {
            return cv::Mat4f(this->m_height,1,this->m_data);
        }

        template<uint32_t _num_points>
        void _VelodyneMatrix<_num_points>::read(boost::filesystem::path path) {
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

        template<uint32_t _num_points>
        bool _VelodyneMatrix<_num_points>::eof() {
            return this->m_eof;
        }

        template class _VelodyneMatrix<VELODYNE_BUFFER_SIZE>;
    }
}