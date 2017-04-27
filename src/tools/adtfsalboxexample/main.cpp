#include <adtf/salbox/Image.h>
#include <adtf/salbox/Gaze.h>
#include <adtf/salbox/TrackedObject.h>
#include <adtf/salbox/VisualPerception.h>
#include <iostream>

#include <core/Core.h>
#include <dlfcn.h>
#include <opencv2/opencv.hpp>
#include <kitti/ImageReader.h>

using namespace std;
using namespace salbox;

using namespace saliency_sandbox::core;
using namespace saliency_sandbox::kitti;


int main(int argc, char* argv[]) {
	INode* pipeline;
	new_pipeline_fun_ptr new_pipeline;
	void* dl;
    LeftRGBImageReader* kitti;
    RGBImage in(LeftRGBImageReader::Image::WIDTH,LeftRGBImageReader::Image::HEIGHT);
    RGBImage* out;

	sserr << sscond(argc < 2) << "expect path to library as first command line argument:\t\tloadpipeline <path to kitti dataset> <path to image>" << ssthrow;
    sserr << sscond(argc < 3) << "expect path to kitti dataset as second command line argument:\t\tloadpipeline <path to protobuild library> <path to kitti dataset>" << ssthrow;

	dl = dlopen(argv[1], RTLD_NOW);
	sserr << sscond(!dl) << "cannot load shared library " << argv[1] << ": " << dlerror() << ssthrow;

	new_pipeline = (new_pipeline_fun_ptr) dlsym(dl, "new_pipeline");
	sserr << sscond(!new_pipeline) << "cannot find entry point of shared library " << argv[1] << ":" << dlerror() << ssthrow;

	pipeline = new_pipeline();
	sserr << sscond(!dl) << "error while creating pipeline" << ssthrow;


    kitti = new LeftRGBImageReader(argv[2]);

    cv::namedWindow("input");
    cv::namedWindow("output");
    cv::startWindowThread();

    for(int time = 0; !kitti->eof(); time++) {
        kitti->template output<0>()->process(time); // read next image
        memcpy( in.buffer,
                kitti->template output<0>()->value()->data(),
                kitti->template output<0>()->value()->bytes()); // copy image

        cv::imshow("input",cv::Mat3b(in.height,in.width,(cv::Vec3b*)in.buffer));

        pipeline->input(0)->data(&in); // pass image to pipeline
        pipeline->output(0)->process(time); // process saliency map
        out = (RGBImage*)pipeline->output(0)->data();

        cv::imshow("output",cv::Mat3b(out->height,out->width,(cv::Vec3b*)out->buffer));
    }
}