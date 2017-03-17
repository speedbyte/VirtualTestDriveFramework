//
// Created by geislerd on 10.02.17.
//

#ifndef VELODYNEREADER_H
#define VELODYNEREADER_H

#include <utils/Image.h>
#include <boost/filesystem/path.hpp>

#define VELODYNE_BUFFER_SIZE 1000000

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
            _VelodyneMatrix();
            virtual ~_VelodyneMatrix();
            uint32_t width() override;
            uint32_t height() override;
            void height(uint32_t height);
            uint32_t typeSize() override;
            uint32_t bytes() override;
            void *data() override;
            cv::Mat mat() override;
            void read(boost::filesystem::path path);
            bool eof();
        };

        template<uint32_t _num_points>
        class _VelodyneReader : public core::Node::template Input<>::template Output<_VelodyneMatrix<_num_points>> {
        public:
            typedef _VelodyneMatrix<_num_points> Matrix;
        private:
            _VelodyneMatrix<_num_points> m_data;
            boost::filesystem::path m_path;

            boost::filesystem::path filename();

            boost::filesystem::path filepath();
        public:
            _VelodyneReader(boost::filesystem::path path);

            void calc() override;

            void reset() override;
        };

        typedef _VelodyneReader<VELODYNE_BUFFER_SIZE> VelodyneReader;
    }
}
#endif //VELODYNEREADER_H
