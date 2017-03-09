//
// Created by geislerd on 10.02.17.
//

#include <core/Pipeline.h>
#include <kitti/ImageReader.h>
#include <io/ImageWriter.h>
#include <kitti/OXTSReader.h>
#include <plot/Plot.h>
#include <io/ImageShow.h>
#include <plot/GridLayout.h>


void process(bool saveOutput, boost::filesystem::path dataset, boost::filesystem::path output);

int main(int argc, char* argv[]) {
    boost::filesystem::path dataset;
    boost::filesystem::path output;
    std::string in;

    if(argc < 3 || argc > 4) {
        std::cout << "plottracklets [kitti database root] [dataset] [save]" << std::endl;
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
        //cv::destroyAllWindows();
        process(argc == 4, dataset, output);
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

void process(bool saveOutput, boost::filesystem::path dataset, boost::filesystem::path output) {
    saliency_sandbox::core::Pipeline pipeline;
    saliency_sandbox::kitti::TrackletReader tracklet_reader(dataset);
    saliency_sandbox::kitti::CalibrationReader calibration_reader(dataset);
    saliency_sandbox::kitti::LeftGrayImageReader left_gray_image_reader(dataset);
    saliency_sandbox::kitti::LeftRGBImageReader left_rgb_image_reader(dataset);
    saliency_sandbox::kitti::RightGrayImageReader right_gray_image_reader(dataset);
    saliency_sandbox::kitti::RightRGBImageReader right_rgb_image_reader(dataset);
    saliency_sandbox::kitti::LeftGrayImageReader::DrawTracklet left_gray_image_draw_tracklet;
    saliency_sandbox::kitti::LeftRGBImageReader::DrawTracklet left_rgb_image_draw_tracklet;
    saliency_sandbox::kitti::RightGrayImageReader::DrawTracklet right_gray_image_draw_tracklet;
    saliency_sandbox::kitti::RightRGBImageReader::DrawTracklet right_rgb_image_draw_tracklet;
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
}