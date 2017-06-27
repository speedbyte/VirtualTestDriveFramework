//
// Created by veikas on 24.06.17.
//

#ifndef SALIENCYSANDBOX2_0_RUNCONFIGURATIONS_H
#define SALIENCYSANDBOX2_0_RUNCONFIGURATIONS_H

#include <filesystem/path.hpp>

namespace saliency_sandbox {
    namespace run_configurations {

        class IRunConfigurations {
        protected:
            std::string m_version_author;
            std::string m_version_copyright;
            std::string m_version_name;
        public:
            IRunConfigurations()
            {
                m_version_author = "Not Initialised";
                m_version_name = "Not Initialised";
                m_version_copyright = "Not Initialised";
            }
            virtual void displayVersionInfo(void) = 0;
        };

        class RunConfigurations: public IRunConfigurations {
            /*  file to execute
                mode
                    nightly build
                    startx
                    tty
                options ->
                        input database
                    output results
                    show graph
                    debugging wait
                    load pipeline
                    static compiling or just in time compiling -> protobuf file
                    using QT
                    test mode
             */
        private:
            std::string m_app_name;
        public:
            boost::filesystem::path m_dataset;
            boost::filesystem::path m_output_directory;
        public:
            RunConfigurations(std::string app_name = "App name not defined"):IRunConfigurations(),
                                                    m_app_name(app_name)
            { }
            void version(const std::string &version_name,
                         const std::string &version_author, const std::string &version_copyright);
            void help(const std::string &app_name);
            void displayVersionInfo(void) override;
            void prepareRunConfiguration(boost::filesystem::path a, boost::filesystem::path b );
        };
    }
}


#endif //SALIENCYSANDBOX2_0_RUNCONFIGURATIONS_H
