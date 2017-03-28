//
// Created by geislerd on 08.02.17.
//

#ifndef IMAGEMEMORYWRITER_H
#define IMAGEMEMORYWRITER_H

#include <config.h>
#include <cstdint>
#include <core/Node.h>
#include <utils/Matrix.h>

namespace saliency_sandbox {
    namespace io {

        class ImageMemoryWriter : public core::Node::template Input<utils::IMatrix>::template Output<cv::Mat> {
        private:
            cv::Mat m_mat;
        public:
            ImageMemoryWriter();

            virtual void calc() override;

            virtual void reset() override;
        };
    }
}

#endif //IMAGEMEMORYWRITER_H
