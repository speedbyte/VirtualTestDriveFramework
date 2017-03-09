//
// Created by geislerd on 08.02.17.
//

#ifndef IMAGESHOW_H
#define IMAGESHOW_H

#include <config.h>
#include <cstdint>
#include <core/Node.h>
#include <utils/Matrix.h>

namespace saliency_sandbox {
    namespace io {

        class ImageShow : public core::Node::template Input<utils::IMatrix>::template Output<> {
        private:
            std::string m_window;
        public:
            ImageShow(std::string window);
            virtual ~ImageShow();

            virtual void calc() override;

            virtual void reset() override;
        };
    }
}

#endif //IMAGESHOW_H
