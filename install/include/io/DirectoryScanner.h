//
// Created by geislerd on 13.02.17.
//

#ifndef DIRECTORYSCANNER_H
#define DIRECTORYSCANNER_H

#include <boost/filesystem.hpp>
#include <core/Node.h>

namespace saliency_sandbox {
    namespace io {
        class DirectoryScanner : public saliency_sandbox::core::Node::template Input<>::template Output<boost::filesystem::path> {
        private:
            boost::filesystem::path m_path;
            std::vector<boost::filesystem::path> m_files;
            std::vector<boost::filesystem::path>::iterator m_iter;
            boost::filesystem::path m_current;

            bool isImageExtension(boost::filesystem::path ext);

            void scanDir(boost::filesystem::path dir);
        public:

            DirectoryScanner(boost::filesystem::path path);

            void calc() override;

            void reset() override;
        };
    }
}
#endif //DIRECTORYSCANNER_H
