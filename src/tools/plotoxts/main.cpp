//
// Created by geislerd on 10.02.17.
//

#include <core/Pipeline.h>
#include <kitti/VelodyneReader.h>
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
        std::cout << "plotoxts [kitti database root] [dataset] [save]" << std::endl;
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