//
// Created by veikas on 24.06.17.
//
#include <iostream>
#include "run_configurations/RunConfigurations.h"


using std::cout;
using boost::filesystem::path;

void saliency_sandbox::run_configurations::RunConfigurations::displayVersionInfo() {
    std::cout << "App Name : " << m_app_name << std::endl;
    std::cout << "Copyright : " << m_version_copyright << std::endl;
    std::cout << "Version : " << m_version_name << std::endl;
    std::cout << "Author : " << m_version_author << std::endl;

    std::cout << std::endl;
}

void saliency_sandbox::run_configurations::RunConfigurations::help(const std::string &app_name) {
    // print usage
    std::cout << "Usage:" << std::endl;
    std::cout << "\t" << app_name << " [options...]" << std::endl << std::endl;
    // print options
    std::cout << "Options:" << std::endl;
    //opt_desc.print(std::cout);
}

void saliency_sandbox::run_configurations::RunConfigurations::version(const std::string &version_name,
                                                                      const std::string &version_author, const std::string &version_copyright) {
    m_version_copyright = version_copyright;
    m_version_name = version_name;
    m_version_author = version_author;
}

void saliency_sandbox::run_configurations::RunConfigurations::prepareRunConfiguration(boost::filesystem::path dataset,
                                                                                      boost::filesystem::path output_directory)  {
    m_dataset = dataset;
    m_output_directory = output_directory;
}