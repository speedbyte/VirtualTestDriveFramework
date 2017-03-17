//
// Created by geislerd on 10.02.17.
//

#include <core/Pipeline.h>
#include <kitti/VelodyneReader.h>
#include <io/ImageShow.h>
#include <io/ImageWriter.h>
#include <kitti/Velodyne2PolarImage.h>


void process(bool saveOutput, boost::filesystem::path dataset, boost::filesystem::path output);

int main(int argc, char* argv[]) {
    boost::filesystem::path dataset;
    boost::filesystem::path output;
    std::string in;

    if(argc < 3 || argc > 4) {
        std::cout << "plotvelodyne [kitti database root] [dataset] [save]" << std::endl;
        exit(-1);
    }

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
    saliency_sandbox::core::Pipeline pipeline;
    saliency_sandbox::kitti::VelodyneReader velodyne_reader(dataset);
    saliency_sandbox::kitti::Velodyne2PolarImage v2pi;
    saliency_sandbox::kitti::VelodynePolarImage::ConvertRGB h2rgb;
    saliency_sandbox::io::ImageShow is(dataset.string());
    saliency_sandbox::io::ImageWriter iw(output.string());

    std::cout << "create pipeline" << std::endl;
    connect_port(velodyne_reader,0,v2pi,0);
    connect_port(v2pi,1,h2rgb,0);
    connect_port(h2rgb,0,is,0);

    pipeline.pushNode("kitti dataset reader",&velodyne_reader);
    pipeline.pushNode("velodyne to polar",&v2pi);
    pipeline.pushNode("polar heatmap",&h2rgb);
    pipeline.pushNode("show heatmap",&is);

    if(saveOutput) {
        connect_port(h2rgb,0,iw,0);
        pipeline.pushNode("write heatmap",&iw);
    }

    std::cout << "initialize pipeline" << std::endl;
    is.properties()->set<bool>("close_window",false);
    pipeline.initialize();

    std::cout << "process pipeline" << std::endl;
    for(time_t time = 0; !pipeline.eof(); time++)
        pipeline.process(time);
}