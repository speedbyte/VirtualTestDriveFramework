//
// Created by geislerd on 10.02.17.
//

#include <core/Pipeline.h>
#include <io/ImageShow.h>
#include <kitti/ImageReader.h>
#include <saliency/activation/Spectral.h>
#include <utils/FPSCounter.h>
#include <plot/Plot.h>


void process(bool saveOutput, boost::filesystem::path dataset, boost::filesystem::path output);

int main(int argc, char* argv[]) {
    boost::filesystem::path dataset;
    boost::filesystem::path output;
    std::string in;

    if(argc < 3 || argc > 4) {
        std::cout << "spectralwhitening [kitti database root] [dataset] [save]" << std::endl;
        exit(-1);
    }

    saliency_sandbox::core::Utils::setMainStackSize();

    dataset /= argv[1];
    dataset /= dataset.parent_path().leaf();
    dataset += "_drive_";
    dataset += argv[2];
    dataset += "_sync/";

    std::cout << "dataset: " << dataset << std::endl;

    if(argc == 4)
        output += argv[3];
    else
        output += argv[0];

    while(true) {
        cv::destroyAllWindows();
        process(argc == 4, dataset, output);
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

void process(bool saveOutput, boost::filesystem::path dataset, boost::filesystem::path output) {
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