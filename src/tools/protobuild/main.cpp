
#include <iostream>
#include <dlfcn.h>
#include <opencv2/opencv.hpp>

#include <core/Core.h>
#include <core/Pipeline.h>

#include <core/Utils.h>
#include <core/Pipeline.h>
#include <core/CodeGen.h>

#include <io/ImageWriter.h>
#include <io/ImageShow.h>

#include <utils/Image.h>
#include <utils/kbhit.h>

#include <plot/Plot.h>
#include <plot/GridLayout.h>

#include <kitti/ImageReader.h>
#include <kitti/OXTSReader.h>
#include <kitti/DrawTracklet.h>
#include <kitti/VelodyneReader.h>
#include <kitti/Velodyne2PolarImage.h>

#include <saliency/activation/Spectral.h>
#include <saliency/activation/BooleanMaps.h>

#include <utils/FPSCounter.h>

//#include <adtf/salbox/Image.h>
//#include <adtf/salbox/Gaze.h>
//#include <adtf/salbox/TrackedObject.h>
//#include <adtf/salbox/VisualPerception.h>

#include <kitti/KittiReader.h>
#include <sensor_fusion/PriorityGraph.h>

/* protobuild generated header defintions*/

#include <src/core/node.pb.h>
#include <src/tools/protobuild/build.h>

// argument parser
#include <boost/program_options.hpp>

#include <run_configurations/RunConfigurations.h>

using namespace std;
//using namespace salbox;

using namespace saliency_sandbox::core;
using namespace saliency_sandbox::kitti;
using namespace google::protobuf;
using namespace google::protobuf::io;

namespace po = boost::program_options;
namespace fs = boost::filesystem;

typedef enum {
    PROTOBUILD = 0,
    PLOTVELODYNE,
    SPECTRALWHITENING,
    BOOLEANMAPS,
    PLOTOXTS,
    PLOTTRACKLETS,
    PRIORITYGRAPH,
    EXIT=10
} TESTCASES;

void process_prioritygraph(bool saveOutput, boost::filesystem::path dataset, boost::filesystem::path output);
void process_plotvelodyne(bool saveOutput, boost::filesystem::path dataset, boost::filesystem::path output);
void process_spectralwhitening(bool saveOutput, boost::filesystem::path dataset, boost::filesystem::path output);
void process_booleanmaps(bool saveOutput, boost::filesystem::path dataset, boost::filesystem::path output);
void process_plotoxts(bool saveOutput, boost::filesystem::path dataset, boost::filesystem::path output);
void process_plottracklets(bool saveOutput, boost::filesystem::path dataset, boost::filesystem::path output);
void process_protobuild(bool saveOutput, boost::filesystem::path dataset, boost::filesystem::path output);


int main( int argc, char *argv[])
{
    int runCommand = 0;
    if ( argc == 1 ) {
        std::cout << "Nothing input..Aborting" << std::endl;
        runCommand=EXIT;
    }
    else {
        if ( strcmp(argv[4], "plotvelodyne") == 0 ) runCommand = PLOTVELODYNE;
        else if ( strcmp(argv[4], "spectralwhitening") == 0 ) runCommand = SPECTRALWHITENING;
        else if ( strcmp(argv[4], "booleanmaps") == 0 ) runCommand = BOOLEANMAPS;
        else if ( strcmp(argv[4], "plotoxts") == 0 ) runCommand = PLOTOXTS;
        else if ( strcmp(argv[4], "prioritygraph") == 0 ) runCommand = PRIORITYGRAPH;
        else if ( strcmp(argv[4], "protobuild") == 0 ) runCommand = PROTOBUILD;
    }

    boost::filesystem::path dataset;
    boost::filesystem::path output;
    std::string in;

    if ( runCommand == PROTOBUILD) {
    }
    else {
        if(argc < 3 || argc > 5) {
            std::cout << "plotvelodyne [kitti database root] [dataset] [save]" << std::endl;
            exit(-1);
        }
        dataset /= argv[1];
        dataset /= dataset.parent_path().leaf();
        dataset += "_drive_";
        dataset += argv[2];
        dataset += "_sync/";

        std::cout << "dataset: " << dataset << std::endl;
        if(argc == 5)
            output += argv[3];
        else
            output += argv[0];
    }

    typedef void (*create_pipeline_t)(saliency_sandbox::core::Pipeline&);

    std::string app_name;
    po::options_description opt_desc;
    po::positional_options_description positionalOptions;
    po::variables_map vm;
    boost::filesystem::path config_path, source_path, lib_path, root_path, dataset_path, output_path;
    std::string cpp, lib_s;
    std::ofstream source_os;
    std::stringstream compile_ss, libs_ss;
    void* dl;
    create_pipeline_t funp;
    saliency_sandbox::core::Pipeline pipeline;

    app_name = boost::filesystem::basename(argv[0]);
    opt_desc.add_options()
            ("help,h","print help messages")
            ("version,v","show version info")
            ("config,c",po::value<boost::filesystem::path>(&config_path)->required(),"path to pipeline description")
            ("output,o",po::value<boost::filesystem::path>(),"place the compiled output into <file>")
            ("run,r","run pipeline after compilation")
            ("show,s","show pipeline graph")
            ("load,l","load already compiled library if exists")
            ("framework,f",po::value<boost::filesystem::path>(),"path to install directory")
            ("wait,w","wait for key press after pipeline iteration");

    switch ( runCommand )
    {
        case PLOTVELODYNE:
        {
                while(true) {
                    cv::destroyAllWindows();
                    process_plotvelodyne(argc == 4, dataset, output);
                    while(true) {
                        std::cout << "pipeline finished or eof" << std::endl;
                        std::cout << "\trerun (y/n):";
                        std::cin >> in;
                        if(in == "y" || in == "Y" || in == "yes" || in == "YES" || in == "Yes" || in.empty())
                            break;
                        else if(in == "n" || in == "N" || in == "no" || in == "NO" || in == "No" || in.empty())
                            exit(0);
                    }
                }
        }
        case SPECTRALWHITENING:
        {
            saliency_sandbox::core::Utils::setMainStackSize();
            while(true) {
                cv::destroyAllWindows();
                process_spectralwhitening(argc == 4, dataset, output);
                while(true) {
                    std::cout << "pipeline finished or eof" << std::endl;
                    std::cout << "\trerun (Y/n):";

                    std::cin >> in;
                    if(in == "y" || in == "Y" || in == "yes" || in == "YES" || in == "Yes" || in.empty())
                        break;
                    else if(in == "n" || in == "N" || in == "no" || in == "NO" || in == "No" || in.empty())
                        exit(0);
                }
            }
        }
        case BOOLEANMAPS:
        {
            while(true) {
                cv::destroyAllWindows();
                process_booleanmaps(argc == 4, dataset, output);
                while(true) {
                    std::cout << "pipeline finished or eof" << std::endl;
                    std::cout << "\trerun (Y/n):";

                    std::cin >> in;
                    if(in == "y" || in == "Y" || in == "yes" || in == "YES" || in == "Yes" || in.empty())
                        break;
                    else if(in == "n" || in == "N" || in == "no" || in == "NO" || in == "No" || in.empty())
                        exit(0);
                }
            }
        }
        case PLOTOXTS:
        {
            saliency_sandbox::core::Utils::setMainStackSize();
            while(true) {
                cv::destroyAllWindows();
                process_plotoxts(argc == 4, dataset, output);
                while(true) {
                    std::cout << "pipeline finished or eof" << std::endl;
                    std::cout << "\trerun (Y/n):";

                    std::cin >> in;
                    if(in == "y" || in == "Y" || in == "yes" || in == "YES" || in == "Yes" || in.empty())
                        break;
                    else if(in == "n" || in == "N" || in == "no" || in == "NO" || in == "No" || in.empty())
                        exit(0);
                }
            }
        }
        case EXIT:
        {
//            INode* pipeline;
//            new_pipeline_fun_ptr new_pipeline;
//            void* dl;
//            LeftRGBImageReader* kitti;
//            RGBImage in(LeftRGBImageReader::Image::WIDTH,LeftRGBImageReader::Image::HEIGHT);
//            RGBImage* out;
//
//            sserr << sscond(argc < 2) << "expect path to library as first command line argument:\t\tloadpipeline <path to kitti dataset> <path to image>" << ssthrow;
//            sserr << sscond(argc < 3) << "expect path to kitti dataset as second command line argument:\t\tloadpipeline <path to protobuild library> <path to kitti dataset>" << ssthrow;
//
//            dl = dlopen(argv[1], RTLD_NOW);
//            sserr << sscond(!dl) << "cannot load shared library " << argv[1] << ": " << dlerror() << ssthrow;
//
//            new_pipeline = (new_pipeline_fun_ptr) dlsym(dl, "new_pipeline");
//            sserr << sscond(!new_pipeline) << "cannot find entry point of shared library " << argv[1] << ":" << dlerror() << ssthrow;
//
//            pipeline = new_pipeline();
//            sserr << sscond(!dl) << "error while creating pipeline" << ssthrow;
//
//
//            kitti = new LeftRGBImageReader(argv[2]);
//
//            cv::namedWindow("input");
//            cv::namedWindow("output");
//            cv::startWindowThread();
//
//            for(int time = 0; !kitti->eof(); time++) {
//                kitti->template output<0>()->process(time); // read next image
//                memcpy( in.buffer,
//                        kitti->template output<0>()->value()->data(),
//                        kitti->template output<0>()->value()->bytes()); // copy image
//
//                cv::imshow("input",cv::Mat3b(in.height,in.width,(cv::Vec3b*)in.buffer));
//
//                pipeline->input(0)->data(&in); // pass image to pipeline
//                pipeline->output(0)->process(time); // process saliency map
//                out = (RGBImage*)pipeline->output(0)->data();
//
//                cv::imshow("output",cv::Mat3b(out->height,out->width,(cv::Vec3b*)out->buffer));
//            }
        }
        case PLOTTRACKLETS:
        {
            saliency_sandbox::core::Utils::setMainStackSize();
            while(true) {
                //cv::destroyAllWindows();
                process_plottracklets(argc == 4, dataset, output);
                continue;
                while(true) {
                    std::cout << "pipeline finished or eof" << std::endl;
                    std::cout << "\trerun (Y/n):";

                    std::cin >> in;
                    if(in == "y" || in == "Y" || in == "yes" || in == "YES" || in == "Yes" || in.empty())
                        break;
                    else if(in == "n" || in == "N" || in == "no" || in == "NO" || in == "No" || in.empty())
                        exit(0);
                }
            }
        }
        case PRIORITYGRAPH:
        {
            process_prioritygraph(argc == 4, dataset, output);
        }
        case PROTOBUILD:
        {
            try {
                // parse commandline options
                po::store(po::command_line_parser(argc, argv).options(opt_desc).positional(positionalOptions).run(),vm);

                // check for help flag
                if(vm.count("help")) {
                    saliency_sandbox::run_configurations::RunConfigurations::help(app_name);
                    exit(EXIT_SUCCESS);
                }

                // check for version flag
                if(vm.count("version")) {
                    saliency_sandbox::run_configurations::RunConfigurations::version();
                    exit(EXIT_SUCCESS);
                }

                // throw error if there was any error
                // while parsing the arguments
                po::notify(vm);

                // find framework root
                if(vm.count("framework")) {
                    root_path = vm["framework"].as<boost::filesystem::path>();
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
                    source_path = vm["output"].as<boost::filesystem::path>();
                    source_path += ".cpp";
                    lib_path = vm["output"].as<boost::filesystem::path>();
                    lib_path += ".so";
                } else {
                    source_path = "./.tmp_pipeline_gen.cpp";
                    lib_path = "./.tmp_pipeline_gen.so";
                }

                if(!vm.count("load") || !boost::filesystem::exists(lib_path) || !boost::filesystem::is_regular_file(lib_path)) {
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
        default:{
            std::cerr<<"No valid input"<<std::endl;
        }
    }
}


void process_plotvelodyne(bool saveOutput, boost::filesystem::path dataset, boost::filesystem::path output) {
    saliency_sandbox::core::Pipeline pipeline;
    saliency_sandbox::kitti::VelodyneReader velodyne_reader(dataset);
    saliency_sandbox::kitti::Velodyne2PolarImage v2pi;
    saliency_sandbox::utils::PolarHeatmapImage::ConvertRGB h2rgb;
    saliency_sandbox::io::ImageShow is(dataset.string());
    saliency_sandbox::io::ImageWriter iw(output.string());

    std::cout << "create pipeline" << std::endl;
    connect_port(velodyne_reader, 0, v2pi, 0);
    connect_port(v2pi, 1, h2rgb, 0);
    connect_port(h2rgb, 0, is, 0);

    pipeline.pushNode("kitti dataset reader", &velodyne_reader);
    pipeline.pushNode("velodyne to polar", &v2pi);
    pipeline.pushNode("polar heatmap", &h2rgb);
    pipeline.pushNode("show heatmap", &is);

    if (saveOutput) {
        connect_port(h2rgb, 0, iw, 0);
        pipeline.pushNode("write heatmap", &iw);
    }

    std::cout << "initialize pipeline" << std::endl;
    is.properties()->set<bool>("close_window", false);
    pipeline.initialize();

    std::cout << "process pipeline" << std::endl;
    for (time_t time = 0; !pipeline.eof(); time++)
        pipeline.process(time);
}

void process_spectralwhitening(bool saveOutput, boost::filesystem::path dataset, boost::filesystem::path output) {
    const uint32_t WIDTH = uint32_t(saliency_sandbox::kitti::LeftRGBImageReader::Image::WIDTH);
    const uint32_t HEIGHT = uint32_t(saliency_sandbox::kitti::LeftRGBImageReader::Image::HEIGHT);

    saliency_sandbox::core::Pipeline pipeline;
    saliency_sandbox::kitti::LeftRGBImageReader image_reader(dataset);
    saliency_sandbox::kitti::LeftRGBImageReader::Image::ConvertLAB lab;
    saliency_sandbox::kitti::LeftRGBImageReader::Image::ConvertLAB::OutputImage::Splitt lab_splitt;
    saliency_sandbox::saliency::activation::_Spectral<WIDTH,HEIGHT> spectral_l;
    saliency_sandbox::saliency::activation::_Spectral<WIDTH,HEIGHT> spectral_a;
    saliency_sandbox::saliency::activation::_Spectral<WIDTH,HEIGHT> spectral_b;
    saliency_sandbox::utils::_Matrix<WIDTH,HEIGHT,cv::Vec3f>::Merge spectral_lab;
    saliency_sandbox::utils::_Matrix<WIDTH,HEIGHT,cv::Vec3f>::Sum spectral;
    saliency_sandbox::utils::_HeatmapImage<WIDTH,HEIGHT>::ConvertRGB rgb;
    saliency_sandbox::io::ImageShow show("Spectral Whitening");

    saliency_sandbox::utils::FPSCounter fps;
    saliency_sandbox::plot::Plot fps_plot;
    saliency_sandbox::io::ImageShow fps_show("FPS - AVG 10");

    std::cout << "create pipeline" << std::endl;
    connect_port(image_reader,0,lab,0);
    connect_port(lab,0,lab_splitt,0);
    connect_port(lab_splitt,0,spectral_l,0);
    connect_port(lab_splitt,1,spectral_a,0);
    connect_port(lab_splitt,2,spectral_b,0);
    connect_port(spectral_l,0,spectral_lab,0);
    connect_port(spectral_a,0,spectral_lab,1);
    connect_port(spectral_b,0,spectral_lab,2);
    connect_port(spectral_lab,0,spectral,0);
    connect_port(spectral,0,rgb,0);
    connect_port(rgb,0,show,0);

    connect_port(rgb,0,fps,0);
    connect_port(fps,0,fps_plot,0);
    connect_port(fps_plot,0,fps_show,0);

    pipeline.pushNode(&image_reader);
    pipeline.pushNode(&lab);
    pipeline.pushNode(&lab_splitt);
    pipeline.pushNode(&spectral_l);
    pipeline.pushNode(&spectral_a);
    pipeline.pushNode(&spectral_b);
    pipeline.pushNode(&spectral_lab);
    pipeline.pushNode(&spectral);
    pipeline.pushNode(&rgb);
    pipeline.pushNode(&show);
    pipeline.pushNode(&fps_show);

    pipeline.pushNode(&fps);
    pipeline.pushNode(&fps_plot);

    std::cout << "initialize pipeline" << std::endl;
    show.properties()->set<bool>("close_window",false);
    pipeline.initialize();

    std::cout << "process pipeline" << std::endl;
    for(time_t time = 0; !pipeline.eof(); time++)
        pipeline.process(time);
}

void process_booleanmaps(bool saveOutput, boost::filesystem::path dataset, boost::filesystem::path output) {
    const uint32_t WIDTH = uint32_t(saliency_sandbox::kitti::LeftRGBImageReader::Image::WIDTH);
    const uint32_t HEIGHT = uint32_t(saliency_sandbox::kitti::LeftRGBImageReader::Image::HEIGHT);

    saliency_sandbox::core::Pipeline pipeline;
    saliency_sandbox::kitti::LeftRGBImageReader image_reader(dataset);
    saliency_sandbox::kitti::LeftRGBImageReader::Image::ConvertLAB lab;
    saliency_sandbox::kitti::LeftRGBImageReader::Image::ConvertLAB::OutputImage::Splitt lab_splitt;
    saliency_sandbox::utils::_HeatmapImage<WIDTH,HEIGHT>::ConvertIntensity int_l;
    saliency_sandbox::utils::_HeatmapImage<WIDTH,HEIGHT>::ConvertIntensity int_a;
    saliency_sandbox::utils::_HeatmapImage<WIDTH,HEIGHT>::ConvertIntensity int_b;
    saliency_sandbox::saliency::activation::_BooleanMaps<WIDTH,HEIGHT> bm_l;
    saliency_sandbox::saliency::activation::_BooleanMaps<WIDTH,HEIGHT> bm_a;
    saliency_sandbox::saliency::activation::_BooleanMaps<WIDTH,HEIGHT> bm_b;
    saliency_sandbox::utils::_Matrix<WIDTH,HEIGHT,cv::Vec3f>::Merge bm_lab;
    saliency_sandbox::utils::_Matrix<WIDTH,HEIGHT,cv::Vec3f>::Sum bm;
    saliency_sandbox::utils::_HeatmapImage<WIDTH,HEIGHT>::ConvertRGB rgb;
    saliency_sandbox::io::ImageShow show("Boolean Maps");

    saliency_sandbox::utils::FPSCounter fps;
    saliency_sandbox::plot::Plot fps_plot;
    saliency_sandbox::io::ImageShow fps_show("FPS - AVG 10");

    std::cout << "create pipeline" << std::endl;
    connect_port(image_reader,0,lab,0);
    connect_port(lab,0,lab_splitt,0);
    connect_port(lab_splitt,0,int_l,0);
    connect_port(lab_splitt,1,int_a,0);
    connect_port(lab_splitt,2,int_b,0);
    connect_port(int_l,0,bm_l,0);
    connect_port(int_a,0,bm_a,0);
    connect_port(int_b,0,bm_b,0);
    connect_port(bm_l,0,bm_lab,0);
    connect_port(bm_a,0,bm_lab,1);
    connect_port(bm_b,0,bm_lab,2);
    connect_port(bm_lab,0,bm,0);
    connect_port(bm,0,rgb,0);
    connect_port(rgb,0,show,0);

    connect_port(rgb,0,fps,0);
    connect_port(fps,0,fps_plot,0);
    connect_port(fps_plot,0,fps_show,0);

    pipeline.pushNode(&image_reader);
    pipeline.pushNode(&lab);
    pipeline.pushNode(&lab_splitt);
    pipeline.pushNode(&bm_l);
    pipeline.pushNode(&bm_a);
    pipeline.pushNode(&bm_b);
    pipeline.pushNode(&bm_lab);
    pipeline.pushNode(&bm);
    pipeline.pushNode(&rgb);
    pipeline.pushNode(&show);
    pipeline.pushNode(&fps_show);
    pipeline.pushNode(&fps);
    pipeline.pushNode(&fps_plot);

    std::cout << "initialize pipeline" << std::endl;
    show.properties()->set<bool>("close_window",false);
    pipeline.initialize();

    std::cout << "process pipeline" << std::endl;
    for(time_t time = 0; !pipeline.eof(); time++)
        pipeline.process(time);
}//


void process_plotoxts(bool saveOutput, boost::filesystem::path dataset, boost::filesystem::path output) {
    saliency_sandbox::core::Pipeline pipeline;
    saliency_sandbox::kitti::OXTSReader oxts_reader(dataset);
    saliency_sandbox::kitti::OXTSRow::Selector<0> oxts_latitude;
    saliency_sandbox::kitti::OXTSRow::Selector<1> oxts_longitude;
    saliency_sandbox::kitti::OXTSRow::Selector<2> oxts_altitude;
    saliency_sandbox::kitti::OXTSRow::Selector<3> oxts_roll;
    saliency_sandbox::kitti::OXTSRow::Selector<4> oxts_pitch;
    saliency_sandbox::kitti::OXTSRow::Selector<5> oxts_heading;
    saliency_sandbox::kitti::OXTSRow::Selector<8> oxts_forward_velocity;
    saliency_sandbox::kitti::OXTSRow::Selector<9> oxts_leftward_velocity;
    saliency_sandbox::kitti::OXTSRow::Selector<10> oxts_upward_velocity;
    saliency_sandbox::plot::Plot plot_latitude("latitude");
    saliency_sandbox::plot::Plot plot_longitude("longitude");
    saliency_sandbox::plot::Plot plot_altitude("altitude");
    saliency_sandbox::plot::Plot plot_roll("roll");
    saliency_sandbox::plot::Plot plot_pitch("pitch");
    saliency_sandbox::plot::Plot plot_heading("heading");
    saliency_sandbox::plot::Plot plot_forward("forward");
    saliency_sandbox::plot::Plot plot_leftward("leftward");
    saliency_sandbox::plot::Plot plot_upward("upward");
    saliency_sandbox::plot::GridLayout<3,1> grid_pos;
    saliency_sandbox::plot::GridLayout<3,1> grid_rot;
    saliency_sandbox::plot::GridLayout<3,1> grid_vel;
    saliency_sandbox::io::ImageShow show_pos("Position");
    saliency_sandbox::io::ImageShow show_rot("Rotation");
    saliency_sandbox::io::ImageShow show_vel("Velocity");

    std::cout << "create pipeline" << std::endl;
    connect_port(oxts_reader,0,oxts_latitude,0);
    connect_port(oxts_latitude,0,plot_latitude,0);

    connect_port(oxts_reader,0,oxts_longitude,0);
    connect_port(oxts_longitude,0,plot_longitude,0);

    connect_port(oxts_reader,0,oxts_altitude,0);
    connect_port(oxts_altitude,0,plot_altitude,0);

    connect_port(oxts_reader,0,oxts_roll,0);
    connect_port(oxts_roll,0,plot_roll,0);

    connect_port(oxts_reader,0,oxts_pitch,0);
    connect_port(oxts_pitch,0,plot_pitch,0);

    connect_port(oxts_reader,0,oxts_heading,0);
    connect_port(oxts_heading,0,plot_heading,0);

    connect_port(oxts_reader,0,oxts_forward_velocity,0);
    connect_port(oxts_forward_velocity,0,plot_forward,0);

    connect_port(oxts_reader,0,oxts_leftward_velocity,0);
    connect_port(oxts_leftward_velocity,0,plot_leftward,0);

    connect_port(oxts_reader,0,oxts_upward_velocity,0);
    connect_port(oxts_upward_velocity,0,plot_upward,0);

    connect_port(plot_latitude,0,grid_pos,0);
    connect_port(plot_longitude,0,grid_pos,1);
    connect_port(plot_altitude,0,grid_pos,2);

    connect_port(plot_roll,0,grid_rot,0);
    connect_port(plot_pitch,0,grid_rot,1);
    connect_port(plot_heading,0,grid_rot,2);

    connect_port(plot_forward,0,grid_vel,0);
    connect_port(plot_leftward,0,grid_vel,1);
    connect_port(plot_upward,0,grid_vel,2);

    connect_port(grid_pos,0,show_pos,0);
    connect_port(grid_rot,0,show_rot,0);
    connect_port(grid_vel,0,show_vel,0);

    pipeline.pushNode(&oxts_reader);
    pipeline.pushNode(&oxts_latitude);
    pipeline.pushNode(&oxts_longitude);
    pipeline.pushNode(&oxts_altitude);
    pipeline.pushNode(&oxts_roll);
    pipeline.pushNode(&oxts_pitch);
    pipeline.pushNode(&oxts_heading);
    pipeline.pushNode(&oxts_forward_velocity);
    pipeline.pushNode(&oxts_leftward_velocity);
    pipeline.pushNode(&oxts_upward_velocity);
    pipeline.pushNode(&plot_latitude);
    pipeline.pushNode(&plot_longitude);
    pipeline.pushNode(&plot_altitude);
    pipeline.pushNode(&plot_roll);
    pipeline.pushNode(&plot_pitch);
    pipeline.pushNode(&plot_heading);
    pipeline.pushNode(&plot_forward);
    pipeline.pushNode(&plot_leftward);
    pipeline.pushNode(&plot_upward);
    pipeline.pushNode(&grid_pos);
    pipeline.pushNode(&grid_rot);
    pipeline.pushNode(&grid_vel);
    pipeline.pushNode(&show_pos);
    pipeline.pushNode(&show_rot);
    pipeline.pushNode(&show_vel);

    std::cout << "initialize pipeline" << std::endl;
    show_pos.properties()->set<bool>("close_window",false);
    show_rot.properties()->set<bool>("close_window",false);
    show_vel.properties()->set<bool>("close_window",false);
    pipeline.initialize();

    std::cout << "process pipeline" << std::endl;
    for(time_t time = 0; !pipeline.eof(); time++)
        pipeline.process(time);
}


void process_protobuild(bool saveOutput, boost::filesystem::path dataset, boost::filesystem::path output) {

}

void process_plottracklets(bool saveOutput, boost::filesystem::path dataset, boost::filesystem::path output) {
    saliency_sandbox::core::Pipeline pipeline;
    saliency_sandbox::kitti::TrackletReader tracklet_reader(dataset);
    saliency_sandbox::kitti::CalibrationReader calibration_reader(dataset);
    saliency_sandbox::kitti::LeftGrayImageReader left_gray_image_reader(dataset);
    saliency_sandbox::kitti::LeftRGBImageReader left_rgb_image_reader(dataset);
    saliency_sandbox::kitti::RightGrayImageReader right_gray_image_reader(dataset);
    saliency_sandbox::kitti::RightRGBImageReader right_rgb_image_reader(dataset);
    saliency_sandbox::kitti::LeftGrayDrawTracklet left_gray_image_draw_tracklet;
    saliency_sandbox::kitti::LeftRGBDrawTracklet left_rgb_image_draw_tracklet;
    saliency_sandbox::kitti::RightGrayDrawTracklet right_gray_image_draw_tracklet;
    saliency_sandbox::kitti::RightRGBDrawTracklet right_rgb_image_draw_tracklet;
    saliency_sandbox::plot::GridLayout<2,2> grid;
    saliency_sandbox::io::ImageShow show("Tracklets");

    std::cout << "create pipeline" << std::endl;
    connect_port(left_gray_image_reader,0,left_gray_image_draw_tracklet,0);
    connect_port(tracklet_reader,0,left_gray_image_draw_tracklet,1);
    connect_port(calibration_reader,0,left_gray_image_draw_tracklet,2);

    connect_port(left_rgb_image_reader,0,left_rgb_image_draw_tracklet,0);
    connect_port(tracklet_reader,0,left_rgb_image_draw_tracklet,1);
    connect_port(calibration_reader,0,left_rgb_image_draw_tracklet,2);

    connect_port(right_gray_image_reader,0,right_gray_image_draw_tracklet,0);
    connect_port(tracklet_reader,0,right_gray_image_draw_tracklet,1);
    connect_port(calibration_reader,0,right_gray_image_draw_tracklet,2);

    connect_port(right_rgb_image_reader,0,right_rgb_image_draw_tracklet,0);
    connect_port(tracklet_reader,0,right_rgb_image_draw_tracklet,1);
    connect_port(calibration_reader,0,right_rgb_image_draw_tracklet,2);

    connect_port(left_gray_image_draw_tracklet,0,grid,0);
    connect_port(left_rgb_image_draw_tracklet,0,grid,2);
    connect_port(right_gray_image_draw_tracklet,0,grid,1);
    connect_port(right_rgb_image_draw_tracklet,0,grid,3);

    connect_port(grid,0,show,0);

    pipeline.pushNode(&tracklet_reader);
    pipeline.pushNode(&calibration_reader);
    pipeline.pushNode(&left_gray_image_reader);
    pipeline.pushNode(&left_rgb_image_reader);
    pipeline.pushNode(&right_gray_image_reader);
    pipeline.pushNode(&right_rgb_image_reader);
    pipeline.pushNode(&left_gray_image_draw_tracklet);
    pipeline.pushNode(&left_rgb_image_draw_tracklet);
    pipeline.pushNode(&right_gray_image_draw_tracklet);
    pipeline.pushNode(&right_rgb_image_draw_tracklet);
    pipeline.pushNode(&grid);
    pipeline.pushNode(&show);

    std::cout << "initialize pipeline" << std::endl;
    show.properties()->set<bool>("close_window",false);
    pipeline.initialize();

    std::cout << "process pipeline" << std::endl;
    for(time_t time = 0; !pipeline.eof(); time++) {
        pipeline.process(time);
    }
}//

void process_prioritygraph(bool saveOutput, boost::filesystem::path dataset, boost::filesystem::path output) {
    saliency_sandbox::core::Pipeline pipeline;
    saliency_sandbox::kitti::KittiReader kitti_reader(dataset);
    saliency_sandbox::sensor_fusion::Noise noise[5];
    saliency_sandbox::sensor_fusion::PriorityGraph pg;

    std::cout << "create pipeline" << std::endl;
    connect_port(kitti_reader,6,noise[0],0);
    connect_port(kitti_reader,6,noise[1],0);
    connect_port(kitti_reader,6,noise[2],0);
    connect_port(kitti_reader,6,noise[3],0);
    connect_port(kitti_reader,6,noise[4],0);
    connect_port(noise[0],0,pg,0);
    connect_port(noise[1],0,pg,1);
    connect_port(noise[2],0,pg,2);
    connect_port(noise[3],0,pg,3);
    connect_port(noise[4],0,pg,4);

    pipeline.pushNode(&kitti_reader);
    pipeline.pushNode(&noise[0]);
    pipeline.pushNode(&noise[1]);
    pipeline.pushNode(&noise[2]);
    pipeline.pushNode(&noise[3]);
    pipeline.pushNode(&noise[4]);
    pipeline.pushNode(&pg);

    std::cout << "initialize pipeline" << std::endl;
    pipeline.initialize();

    std::cout << "process pipeline" << std::endl;
    for(time_t time = 0; !pipeline.eof(); time++)
        for(uint32_t i = 0; i < pipeline.numOutput(); i++)
            pipeline.output(i)->process(time);
    pipeline.show();
    while (!kbhit())
        usleep(100);
}
