//
// Created by geislerd on 21.02.17.
//

#include <utils/Error.h>

namespace saliency_sandbox {
    namespace utils {

        Error::Error(const char* prefix) : m_ss(),  m_valid(true), m_prefix(prefix)  {
        }

        Error& Error::operator<<(const fun_ptr fun) {
            if(this->m_valid)
                (this->*fun)();
            return *this;
        }

        std::string Error::toString() {
            return this->m_ss.str();
        }

        void Error::invalidate() {
            this->m_valid = false;
        }

        void Error::stub() {

        }

        void Error::runtime_exception() {
            if(this->m_valid)
                throw std::runtime_error(std::string(this->m_prefix) + this->m_ss.str());
        }

        Error::fun_ptr Error::condition(const bool& cond) {
            if(!cond)
                return &Error::invalidate;
            return &Error::stub;
        }

        Error::fun_ptr Error::path_exists(const boost::filesystem::path& path) {
            return Error::condition(!(boost::filesystem::exists(path)));
        }

        Error::fun_ptr Error::is_file(const boost::filesystem::path& path) {
            return Error::condition(!boost::filesystem::exists(path) || !boost::filesystem::is_regular_file(path));
        }

        Error::fun_ptr Error::is_directory(const boost::filesystem::path& path) {
            return Error::condition(!boost::filesystem::exists(path) || !boost::filesystem::is_directory(path));
        }

    }
}