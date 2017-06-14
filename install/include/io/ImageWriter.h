//
// Created by geislerd on 08.02.17.
//

#ifndef IMAGEWRITER_H
#define IMAGEWRITER_H

#include <config.h>
#include <cstdint>
#include <core/Node.h>
#include <utils/Matrix.h>
#include <boost/filesystem.hpp>

namespace saliency_sandbox {
    namespace io {

        class ImageWriter : public core::Node::template Input<utils::IMatrix>::template Output<> {
        private:
            boost::filesystem::path m_path;
        public:
            ImageWriter(boost::filesystem::path path);

            virtual void calc() override;

            virtual void reset() override;
        };
    }
}

#endif //IMAGEWRITER_H
