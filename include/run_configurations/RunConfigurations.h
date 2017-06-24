//
// Created by veikas on 24.06.17.
//

#ifndef SALIENCYSANDBOX2_0_RUNCONFIGURATIONS_H
#define SALIENCYSANDBOX2_0_RUNCONFIGURATIONS_H

namespace saliency_sandbox {
    namespace run_configurations {

        class IRunConfigurations {
        protected:
            std::string m_version;
            IRunConfigurations(std::string m_version) {}
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
            public:
                RunConfigurations(std::string app_name = "Not Defined"):IRunConfigurations(app_name) {};
                static void version();
                static void help(std::string app_name);
        };
    }
}


#endif //SALIENCYSANDBOX2_0_RUNCONFIGURATIONS_H
