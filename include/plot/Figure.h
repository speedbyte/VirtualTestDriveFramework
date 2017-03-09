//
// Created by geislerd on 14.02.17.
//

#ifndef FIGURE_H
#define FIGURE_H

#include <utils/Matrix.h>

namespace saliency_sandbox {
    namespace plot {

        class Figure : public saliency_sandbox::utils::IMatrix {
        private:
            cv::Mat3b m_mat;
        public:
            uint32_t width() override;

            uint32_t height() override;

            uint32_t typeSize() override;

            uint32_t bytes() override;

            void *data() override;

            cv::Mat mat() override;

            Figure &operator<<(const cv::Mat3b &mat);
        };

    }
}

#endif //FIGURE_H
