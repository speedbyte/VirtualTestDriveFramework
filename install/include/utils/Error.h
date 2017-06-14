//
// Created by geislerd on 20.02.17.
//

#ifndef ERROR_H
#define ERROR_H

#include <stdexcept>
#include <ostream>
#include <boost/filesystem.hpp>
#include <sstream>

namespace saliency_sandbox {
    namespace utils {

        class Error {
        public:
            typedef void (Error::*fun_ptr)();
        private:
            std::stringstream m_ss;
            bool m_valid;
            const char* m_prefix;

        public:
            Error(const char* prefix);

            template<typename _type>
            Error& operator<<(const _type& value) {
                if(this->m_valid)
                    this->m_ss << value;
                return *this;
            }

            Error& operator<<(const fun_ptr fun);

            std::string toString();

            void invalidate();

            void stub();

            void runtime_exception();

            static fun_ptr condition(const bool& cond);

            static fun_ptr path_exists(const boost::filesystem::path& path);

            static fun_ptr is_file(const boost::filesystem::path& path);

            static fun_ptr is_directory(const boost::filesystem::path& path);
        };

#define ssprefix "\n\tin file: " __FILE__ ":" BOOST_PP_STRINGIZE(__LINE__) "\n\tmessage:\n\t\t"
#define sserr do { saliency_sandbox::utils::Error( ssprefix )
#define sscond( cond ) saliency_sandbox::utils::Error::condition((cond))
#define ssthrow &saliency_sandbox::utils::Error::runtime_exception; } while(false)
#define sspexits( path ) saliency_sandbox::utils::Error::path_exists( path ) << "no such file or directory: " << path
#define sspfile( path ) saliency_sandbox::utils::Error::is_file( path ) << "file not found: " << path
#define sspdir( path ) saliency_sandbox::utils::Error::is_directory( path ) << "directory not found: " << path
#define ssequal( v0 , v1 ) sscond( (v0) != (v1) ) << "expected: " << (v1) << " got: " << (v0)

    }
}


#endif //ERROR_H
