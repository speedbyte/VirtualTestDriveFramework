//
// Created by geislerd on 14.02.17.
//

#include <plot/Figure.h>

namespace saliency_sandbox {
    namespace plot {

        uint32_t Figure::width() {
            return (uint32_t) this->m_mat.cols;
        }

        uint32_t Figure::height() {
            return (uint32_t) this->m_mat.rows;
        }

        uint32_t Figure::typeSize() {
            return sizeof(cv::Mat3b::value_type);
        }

        uint32_t Figure::bytes() {
            return this->width()*this->height()*this->typeSize();
        }

        void *Figure::data() {
            return this->m_mat.data;
        }

        cv::Mat Figure::mat() {
            return this->m_mat;
        }

        Figure& Figure::operator<<(const cv::Mat3b& mat) {
            this->m_mat = mat;
            return *this;
        }
    }
}