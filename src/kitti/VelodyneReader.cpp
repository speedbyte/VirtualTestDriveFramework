//
// Created by geislerd on 16.03.17.
//

#include <kitti/VelodyneReader.h>

namespace saliency_sandbox {
    namespace kitti {
        template<uint32_t _num_points>
        boost::filesystem::path _VelodyneReader<_num_points>::filename() {
            char filename[16];

            sprintf(filename,"%010ld.bin",this->time());

            return boost::filesystem::path(filename);
        }

        template<uint32_t _num_points>
        boost::filesystem::path _VelodyneReader<_num_points>::filepath() {
            return this->m_path / "velodyne_points" / "data" / filename();
        }

        template<uint32_t _num_points>
        _VelodyneReader<_num_points>::_VelodyneReader(boost::filesystem::path path) : m_path(path) {
                this->reset();
                //TODO: check path
        }

        template<uint32_t _num_points>
        void _VelodyneReader<_num_points>::calc() {
            this->m_data.read(this->filepath());
            this->output()->value(&(this->m_data));

            this->eof(this->m_data.eof());
        }

        template<uint32_t _num_points>
        void _VelodyneReader<_num_points>::reset() { }

        template class _VelodyneReader<VELODYNE_BUFFER_SIZE>;
    }
}