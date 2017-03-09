//
// Created by geislerd on 08.02.17.
//

#include <io/VideoWriter.h>

namespace saliency_sandbox {
    namespace io {

        VideoWriter::VideoWriter(boost::filesystem::path path) : core::Node::template Input<utils::IMatrix>::template Output<>(), m_path(path) {
            //TODO: check path (extension)
            if(path.has_parent_path() && !boost::filesystem::exists(path.parent_path()))
                boost::filesystem::create_directories(path.parent_path());

            this->reset();

            this->template input()->name("frame");
        }

        VideoWriter::~VideoWriter() {
            if(this->m_vw.isOpened())
                this->m_vw.release();
        }

        void VideoWriter::calc() {
            if(!this->m_vw.isOpened())
                this->reset();

            this->m_vw << this->input()->value()->mat();
        }

        void VideoWriter::reset() {
            if(this->m_vw.isOpened())
                this->m_vw.release();
            this->m_vw = cv::VideoWriter();
            if(this->input()->value() != nullptr) {
                //TODO: codec and framerate as properties
                this->m_vw.open(this->m_path.string(), this->properties()->get("codec",CV_FOURCC('M', 'J', 'P', 'G')), this->properties()->get("fps",60.0f), this->input()->value()->mat().size());
            }
        }
    }
}