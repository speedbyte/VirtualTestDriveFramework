//
// Created by geislerd on 13.03.17.
//

#include <io/VideoReader.h>

namespace saliency_sandbox {
    namespace io {
        _VideoReaderImpl::_VideoReaderImpl(boost::filesystem::path path) : m_vc(), m_path(path) {
            sserr << sspfile(path) << ssthrow;
            this->reopen();
        }

        void _VideoReaderImpl::reopen() {
            if (this->m_vc.isOpened())
                this->m_vc.release();
            this->m_vc = cv::VideoCapture();
            this->m_vc.open(this->m_path.c_str());
        }

        bool _VideoReaderImpl::read(cv::Mat3b &mat) {
            cv::Mat3b raw;

            this->m_vc >> raw;

            if (raw.size[0] > 0 && raw.size[1] > 0) {
                cv::resize(raw, mat, mat.size());
                return true;
            } else
                return false;
        }
    }
}