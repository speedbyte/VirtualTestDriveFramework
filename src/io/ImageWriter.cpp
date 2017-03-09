//
// Created by geislerd on 08.02.17.
//

#include <io/ImageWriter.h>

namespace saliency_sandbox {
    namespace io {

        ImageWriter::ImageWriter(boost::filesystem::path path) : core::Node::template Input<utils::IMatrix>::template Output<>(), m_path(path) {
            this->reset();

            // create path if not exists
            if(!boost::filesystem::exists(path))
                boost::filesystem::create_directories(path);

            sserr << sspdir(path) << "Cannot create path: " << path << ssthrow;

            // setup input port
            this->template input<0>()->name("image");
            this->template input<0>()->checkPortSize(false);
        }

        void ImageWriter::calc() {
            char filename[256];
            int n;
            bool s;

            n = snprintf(filename,sizeof(filename),"%s_%08ld.tiff",this->input()->name(),this->input()->time());
            sserr << sscond(n >= sizeof(filename) || n <= 0) << "Error while generating filename: " << filename << ssthrow;


            s = cv::imwrite((this->m_path / filename).string(), this->input()->value()->mat());
            sserr << sscond(!s) << "Cannot write file: " << (this->m_path / filename).string() << ssthrow;
        }

        void ImageWriter::reset() {

        }
    }
}