//
// Created by geislerd on 08.03.17.
//

#include <io/DirectoryScanner.h>

namespace saliency_sandbox {
    namespace io {

        bool DirectoryScanner::isImageExtension(boost::filesystem::path ext) {
            return true; //TODO: check path extension
        }

        void DirectoryScanner::scanDir(boost::filesystem::path dir) {
            boost::filesystem::directory_iterator end, iter(dir);
            boost::filesystem::path val;

            for(; iter != end; iter++) {
                val = *iter;
                if(boost::filesystem::is_directory(val)) {
                    if(val.filename() != "." && val.filename() != "..")
                        this->scanDir(val);
                    else
                        ; // ignore "." and ".."
                } else if(boost::filesystem::is_regular_file(val)) {
                    if(this->isImageExtension(val.extension()))
                        this->m_files.push_back(val);
                    else
                        ; // ignore non image files
                } else
                    ; // ignore non regular files or directories
            }
        }

        DirectoryScanner::DirectoryScanner(boost::filesystem::path path) : m_path(path) {
            this->reset();
        }

        void DirectoryScanner::calc() {
            if(this->m_iter != this->m_files.end()) {
                this->m_current = *(this->m_iter);
                this->output()->value(&(this->m_current));
                this->m_iter++;
            } else {
                this->eof(true);
                this->output()->value(nullptr);
            }
        }

        void DirectoryScanner::reset() {
            this->m_files.clear();
            this->scanDir(this->m_path);
            std::sort(this->m_files.begin(),this->m_files.end());
            this->m_iter = this->m_files.begin();
        }
    }
}