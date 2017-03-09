//
// Created by geislerd on 08.02.17.
//

#ifndef VIDEOWRITER_H
#define VIDEOWRITER_H

#include <config.h>
#include <cstdint>
#include <core/Node.h>
#include <utils/Matrix.h>
#include <boost/filesystem.hpp>

namespace saliency_sandbox {
    namespace io {

        class VideoWriter : public core::Node::template Input<utils::IMatrix>::template Output<> {
        private:
            cv::VideoWriter m_vw;
            boost::filesystem::path m_path;

        public:
            VideoWriter(boost::filesystem::path path);
            virtual ~VideoWriter();

            virtual void calc() override;

            virtual void reset() override;
        };
    }
}

#endif //VIDEOWRITER_H
