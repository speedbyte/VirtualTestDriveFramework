//
// Created by geislerd on 10.02.17.
//

#include "kbhit.h"

#include <src/core/node.pb.h>
#include <src/tools/protobuild/build.h>

#include <core/Utils.h>
#include <core/Pipeline.h>
#include <core/CodeGen.h>

// iostream
#include <fstream>
// dlload
#include <dlfcn.h>

// argument parser
#include <boost/program_options.hpp>

#include <iostream>


using namespace google::protobuf;
using namespace google::protobuf::io;
namespace po = boost::program_options;
namespace fs = boost::filesystem;

typedef void (*create_pipeline_t)(saliency_sandbox::core::Pipeline&);

std::string app_name;
po::options_description opt_desc;
po::positional_options_description positionalOptions;

void version() {
    std::cout << "Saliency Sandbox 2.0 - ProtoBuild" << std::endl;
    std::cout << "\tDavid Geisler - david.geisler@uni-tuebingen.de" << std::endl;
    std::cout << "\tUniversity of Tuebingen (c) 2017" << std::endl << std::endl;
}

void help() {
    // print version string
    version();

    // print usage
    std::cout << "Usage:" << std::endl;
    std::cout << "\t" << app_name << " [options...]" << std::endl << std::endl;

    // print options
    std::cout << "Options:" << std::endl;
    opt_desc.print(std::cout);
}

int main(int argc, char** argv) {
    po::variables_map vm;
    fs::path config_path, source_path, lib_path, root_path;
    std::string cpp, lib_s;
    std::ofstream source_os;
    std::stringstream compile_ss, libs_ss;
    void* dl;
    create_pipeline_t funp;
    saliency_sandbox::core::Pipeline pipeline;

    app_name = fs::basename(argv[0]);
    opt_desc.add_options()
            ("help,h","print help messages")
            ("version,v","show version info")
            ("config,c",po::value<fs::path>(&config_path)->required(),"path to pipeline description")
            ("output,o",po::value<fs::path>(),"place the compiled output into <file>")
            ("run,r","run pipeline after compilation")
            ("show,s","show pipeline graph")
            ("load,l","load already compiled library if exists")
            ("framework,f",po::value<fs::path>(),"path to install directory")
            ("wait,w","wait for key press after pipeline iteration");

    try {
        // parse commandline options
        po::store(po::command_line_parser(argc, argv).options(opt_desc).positional(positionalOptions).run(),vm);

        // check for help flag
        if(vm.count("help")) {
            help();
            exit(EXIT_SUCCESS);
        }

        // check for version flag
        if(vm.count("version")) {
            version();
            exit(EXIT_SUCCESS);
        }

        // throw error if there was any error
        // while parsing the arguments
        po::notify(vm);

        // find framework root
        if(vm.count("framework")) {
            root_path = vm["framework"].as<fs::path>();
        } else {
            root_path = argv[0];
            root_path = root_path.parent_path().parent_path();
        }

        // check fs
        sserr << sspdir(root_path) << "\n\t\tframework root path is not a valid directory: " << root_path << ssthrow;
        sserr << sspdir(root_path / "bin") << "\n\t\tframework root path is not a valid directory: " << root_path << ssthrow;
        sserr << sspdir(root_path / "lib") << "\n\t\tframework root path is not a valid directory: " << root_path << ssthrow;
        sserr << sspdir(root_path / "include") << "\n\t\tframework root path is not a valid directory: " << root_path << ssthrow;
        sserr << sspfile(root_path / "bin" / "protobuild") << "\n\t\tframework root path is not a valid directory: " << root_path << ssthrow;

        // create output paths
        if(vm.count("output")) {
            source_path = vm["output"].as<fs::path>();
            source_path += ".cpp";
            lib_path = vm["output"].as<fs::path>();
            lib_path += ".so";
        } else {
            source_path = "./.tmp_pipeline_gen.cpp";
            lib_path = "./.tmp_pipeline_gen.so";
        }

        if(!vm.count("load") || !fs::exists(lib_path) || !fs::is_regular_file(lib_path)) {
            std::cout << "compile pipeline...";

            // check config path
            sserr << sspfile(config_path) << ssthrow;

            // generate code
            cpp = saliency_sandbox::core::CodeGen::cpp(config_path);

            // save source file
            source_os.open(source_path.c_str());
            source_os << cpp;
            source_os.close();

            // generate compile command
            compile_ss << PROTOBUILD_COMPILER << " ";
            compile_ss << source_path.c_str() << " -o " << lib_path.c_str() << " ";
            compile_ss << PROTOBUILD_COMPILER_FLAGS << " ";
            //compile_ss << "-I" << (root_path / "include") << " ";
            compile_ss << "-I" << PROTOBUILD_COMPILER_INCLUDE << " ";
            compile_ss << "-L" << (root_path / "lib") << " ";


            // collect libraries
            libs_ss = std::stringstream(PROTOBUILD_COMPILER_LIBS);
            while(std::getline(libs_ss,lib_s,';'))
                compile_ss << " " << (root_path / "lib" / lib_s).c_str() << " ";

            // compile library
            sserr << sscond(system(compile_ss.str().c_str())) << "error while compiling pipeline" << ssthrow;
            std::cout << " done" << std::endl;
        }

        if(vm.count("show") || vm.count("run")) {

            // load library
            std::cout << "load pipeline...";
            dl = dlopen(lib_path.c_str(), RTLD_NOW);
            sserr << sscond(!dl) << "cannot load shared library " << lib_path << ":" << dlerror() << ssthrow;

            // get entry
            funp = (create_pipeline_t) dlsym(dl, "create_pipeline");
            sserr << sscond(!funp) << "cannot find entry point of shared library " << lib_path << ":" << dlerror() << ssthrow;
            std::cout << " done" << std::endl;

            // initialize pipeline
            std::cout << "initialize pipeline...";
            funp(pipeline);
            pipeline.initialize();
            std::cout << " done" << std::endl;

            if (vm.count("show")) {
                pipeline.show();
                if (vm.count("wait"))
                    while (!kbhit())
                        usleep(100);
            }

            if (vm.count("run")) {
                std::cout << "run pipeline...";
                std::cout << "" << std::endl;
                for (time_t time = 0; !pipeline.eof(); time++) {
                    pipeline.process(time);
                    if (vm.count("wait"))
                        while (!kbhit())
                            usleep(100);
                }
                std::cout << " done" << std::endl;
            }
        }

    } catch(boost::program_options::required_option& e) {
        sserr << e.what() << "\n\t\ttry " << app_name << " -h to get usage information" << ssthrow;
    } catch(boost::program_options::error& e) {
        sserr << e.what() << "\n\t\ttry " << app_name << " -h to get usage information" << ssthrow;
    }
}