//
// Created by veikas on 24.06.17.
//
#include <iostream>
#include "run_configurations/RunConfigurations.h"

using std::cout;

void saliency_sandbox::run_configurations::RunConfigurations::help(std::string app_name) {
    // print usage
    std::cout << "Usage:" << std::endl;
    std::cout << "\t" << app_name << " [options...]" << std::endl << std::endl;

    // print options
    std::cout << "Options:" << std::endl;
    //opt_desc.print(std::cout);
}

void saliency_sandbox::run_configurations::RunConfigurations::version() {
        std::cout << "Saliency Sandbox 2.0 - ProtoBuild" << std::endl;
        std::cout << "\tDavid Geisler - david.geisler@uni-tuebingen.de" << std::endl;
        std::cout << "\tUniversity of Tuebingen (c) 2017" << std::endl << std::endl;
}