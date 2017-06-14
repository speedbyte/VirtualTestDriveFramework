/*  
 *  
 */ 
#include <core/Pipeline.h>
/* 
 * include header for node "Kitti Reader"
 * instance type: "saliency_sandbox::kitti::KittiReader"
 */
#include <kitti/KittiReader.h>
/* 
 * include header for node "Show Camera 2"
 * instance type: "saliency_sandbox::io::ImageShow"
 */
#include <io/ImageShow.h>
/* 
 * include header for node "OXTS Forward Velocity"
 * instance type: "saliency_sandbox::kitti::OXTSRow::Selector"
 */
#include <kitti/OXTSReader.h>
/* 
 * include header for node "OXTS Leftward Velocity"
 * instance type: "saliency_sandbox::kitti::OXTSRow::Selector"
 */
#include <kitti/OXTSReader.h>
/* 
 * include header for node "OXTS Upward Velocity"
 * instance type: "saliency_sandbox::kitti::OXTSRow::Selector"
 */
#include <kitti/OXTSReader.h>
/* 
 * include header for node "OXTS Velocity Plot"
 * instance type: "saliency_sandbox::plot::Plot"
 */
#include <plot/Plot.h>
/* 
 * include header for node "Show Velocity Plot"
 * instance type: "saliency_sandbox::io::ImageShow"
 */
#include <io/ImageShow.h>
/* 
 * include header for node "OXTS Roll"
 * instance type: "saliency_sandbox::kitti::OXTSRow::Selector"
 */
#include <kitti/OXTSReader.h>
/* 
 * include header for node "OXTS Pitch"
 * instance type: "saliency_sandbox::kitti::OXTSRow::Selector"
 */
#include <kitti/OXTSReader.h>
/* 
 * include header for node "OXTS Yaw"
 * instance type: "saliency_sandbox::kitti::OXTSRow::Selector"
 */
#include <kitti/OXTSReader.h>
/* 
 * include header for node "OXTS forward rate"
 * instance type: "saliency_sandbox::kitti::OXTSRow::Selector"
 */
#include <kitti/OXTSReader.h>
/* 
 * include header for node "OXTS leftward rate"
 * instance type: "saliency_sandbox::kitti::OXTSRow::Selector"
 */
#include <kitti/OXTSReader.h>
/* 
 * include header for node "OXTS upward rate"
 * instance type: "saliency_sandbox::kitti::OXTSRow::Selector"
 */
#include <kitti/OXTSReader.h>
/* 
 * include header for node "OXTS Rotation Plot"
 * instance type: "saliency_sandbox::plot::Plot"
 */
#include <plot/Plot.h>
/* 
 * include header for node "Show Rotation Plot"
 * instance type: "saliency_sandbox::io::ImageShow"
 */
#include <io/ImageShow.h>
/* 
 * include header for node "Kitti Pose"
 * instance type: "saliency_sandbox::kitti::OXTSPose"
 */
#include <kitti/OXTSPose.h>
/* 
 * include header for node "Kitti Ego Motion"
 * instance type: "saliency_sandbox::kitti::EgoMotion"
 */
#include <kitti/EgoMotion.h>
/* 
 * include header for node "Kitti Ego Motion FPS Counter"
 * instance type: "saliency_sandbox::utils::FPSCounter"
 */
#include <utils/FPSCounter.h>
/* 
 * include header for node "Ego Motion Trajectory"
 * instance type: "saliency_sandbox::flow::ExtrapolatePose"
 */
#include <flow/Trajectory.h>
/* 
 * include header for node "Draw Ego Motion Trajectory"
 * instance type: "saliency_sandbox::kitti::DrawTrajectory"
 */
#include <kitti/DrawTrajectory.h>
/* 
 * include header for node "Show Ego Motion Trajectory"
 * instance type: "saliency_sandbox::io::ImageShow"
 */
#include <io/ImageShow.h>
/* 
 * include header for node "Convert to Gray"
 * instance type: "saliency_sandbox::utils::_ImageConvert"
 */
#include <utils/Image.h>
/* 
 * include header for node "Optical Motion"
 * instance type: "saliency_sandbox::flow::_OpticalFlow"
 */
#include <flow/OpticalFlow.h>
/* 
 * include header for node "Optical Motion FPS Counter"
 * instance type: "saliency_sandbox::utils::FPSCounter"
 */
#include <utils/FPSCounter.h>
/* 
 * include header for node "FPS Plot"
 * instance type: "saliency_sandbox::plot::Plot"
 */
#include <plot/Plot.h>
/* 
 * include header for node "Show FPS Plot"
 * instance type: "saliency_sandbox::io::ImageShow"
 */
#include <io/ImageShow.h>
/* 
 * include header for node "Ego Motion Quiver"
 * instance type: "saliency_sandbox::plot::Quiver"
 */
#include <plot/Quiver.h>
/* 
 * include header for node "Show Ego Motion Quiver"
 * instance type: "saliency_sandbox::io::ImageShow"
 */
#include <io/ImageShow.h>
/* 
 * include header for node "Optical Motion Quiver"
 * instance type: "saliency_sandbox::plot::Quiver"
 */
#include <plot/Quiver.h>
/* 
 * include header for node "Show Optical Motion Quiver"
 * instance type: "saliency_sandbox::io::ImageShow"
 */
#include <io/ImageShow.h>

/* 
 * declare instance of node "Kitti Reader"
 */
saliency_sandbox::kitti::KittiReader node_0(std::string("/local/git/PriorityGraphSensors/project/PerceptionSandbox/install/data/kitti/2011_09_26/2011_09_26_drive_0001_sync/"));
/* 
 * declare instance of node "Show Camera 2"
 */
saliency_sandbox::io::ImageShow node_1(std::string("Kitti Cam 0"));
/* 
 * declare instance of node "OXTS Forward Velocity"
 */
saliency_sandbox::kitti::OXTSRow::Selector<uint32_t(8)> node_2;
/* 
 * declare instance of node "OXTS Leftward Velocity"
 */
saliency_sandbox::kitti::OXTSRow::Selector<uint32_t(9)> node_3;
/* 
 * declare instance of node "OXTS Upward Velocity"
 */
saliency_sandbox::kitti::OXTSRow::Selector<uint32_t(10)> node_4;
/* 
 * declare instance of node "OXTS Velocity Plot"
 */
saliency_sandbox::plot::Plot node_5(std::string("Forward"),std::string("Leftward"),std::string("Upward"));
/* 
 * declare instance of node "Show Velocity Plot"
 */
saliency_sandbox::io::ImageShow node_6(std::string("OXTS Velocity"));
/* 
 * declare instance of node "OXTS Roll"
 */
saliency_sandbox::kitti::OXTSRow::Selector<uint32_t(3)> node_7;
/* 
 * declare instance of node "OXTS Pitch"
 */
saliency_sandbox::kitti::OXTSRow::Selector<uint32_t(4)> node_8;
/* 
 * declare instance of node "OXTS Yaw"
 */
saliency_sandbox::kitti::OXTSRow::Selector<uint32_t(5)> node_9;
/* 
 * declare instance of node "OXTS forward rate"
 */
saliency_sandbox::kitti::OXTSRow::Selector<uint32_t(20)> node_10;
/* 
 * declare instance of node "OXTS leftward rate"
 */
saliency_sandbox::kitti::OXTSRow::Selector<uint32_t(21)> node_11;
/* 
 * declare instance of node "OXTS upward rate"
 */
saliency_sandbox::kitti::OXTSRow::Selector<uint32_t(22)> node_12;
/* 
 * declare instance of node "OXTS Rotation Plot"
 */
saliency_sandbox::plot::Plot node_13(std::string("roll"),std::string("pitch"),std::string("yaw"),std::string("forward rate"),std::string("leftward rate"),std::string("upward rate"));
/* 
 * declare instance of node "Show Rotation Plot"
 */
saliency_sandbox::io::ImageShow node_14(std::string("OXTS Rotation"));
/* 
 * declare instance of node "Kitti Pose"
 */
saliency_sandbox::kitti::OXTSPose node_15;
/* 
 * declare instance of node "Kitti Ego Motion"
 */
saliency_sandbox::kitti::EgoMotion<saliency_sandbox::kitti::Camera::LEFT_RGB> node_16;
/* 
 * declare instance of node "Kitti Ego Motion FPS Counter"
 */
saliency_sandbox::utils::FPSCounter node_17;
/* 
 * declare instance of node "Ego Motion Trajectory"
 */
saliency_sandbox::flow::ExtrapolatePose<uint32_t(20)> node_18;
/* 
 * declare instance of node "Draw Ego Motion Trajectory"
 */
saliency_sandbox::kitti::DrawTrajectory<saliency_sandbox::kitti::Camera::LEFT_RGB,uint32_t(20)> node_19;
/* 
 * declare instance of node "Show Ego Motion Trajectory"
 */
saliency_sandbox::io::ImageShow node_20(std::string("Ego Motion Trajectory"));
/* 
 * declare instance of node "Convert to Gray"
 */
saliency_sandbox::utils::_ImageConvert<uint32_t(saliency_sandbox::kitti::LeftRGBImageReader::Image::WIDTH),uint32_t(saliency_sandbox::kitti::LeftRGBImageReader::Image::HEIGHT),saliency_sandbox::utils::_RGBImage,saliency_sandbox::utils::_IntensityImage> node_21;
/* 
 * declare instance of node "Optical Motion"
 */
saliency_sandbox::flow::_OpticalFlow<uint32_t(saliency_sandbox::kitti::LeftRGBImageReader::Image::WIDTH),uint32_t(saliency_sandbox::kitti::LeftRGBImageReader::Image::HEIGHT)> node_22;
/* 
 * declare instance of node "Optical Motion FPS Counter"
 */
saliency_sandbox::utils::FPSCounter node_23;
/* 
 * declare instance of node "FPS Plot"
 */
saliency_sandbox::plot::Plot node_24(std::string("Ego Motion"),std::string("Optical Motion"));
/* 
 * declare instance of node "Show FPS Plot"
 */
saliency_sandbox::io::ImageShow node_25(std::string("FPS"));
/* 
 * declare instance of node "Ego Motion Quiver"
 */
saliency_sandbox::plot::Quiver<uint32_t(saliency_sandbox::kitti::LeftRGBImageReader::Image::WIDTH),uint32_t(saliency_sandbox::kitti::LeftRGBImageReader::Image::HEIGHT)> node_26;
/* 
 * declare instance of node "Show Ego Motion Quiver"
 */
saliency_sandbox::io::ImageShow node_27(std::string("Ego Motion"));
/* 
 * declare instance of node "Optical Motion Quiver"
 */
saliency_sandbox::plot::Quiver<uint32_t(saliency_sandbox::kitti::LeftRGBImageReader::Image::WIDTH),uint32_t(saliency_sandbox::kitti::LeftRGBImageReader::Image::HEIGHT)> node_28;
/* 
 * declare instance of node "Show Optical Motion Quiver"
 */
saliency_sandbox::io::ImageShow node_29(std::string("Optical Motion"));

extern "C" void create_pipeline(saliency_sandbox::core::Pipeline& pipeline) {
	/*
	 * set properties of node "Kitti Reader"
	 */

	/*
	 * set properties of node "Show Camera 2"
	 */

	/*
	 * set properties of node "OXTS Forward Velocity"
	 */

	/*
	 * set properties of node "OXTS Leftward Velocity"
	 */

	/*
	 * set properties of node "OXTS Upward Velocity"
	 */

	/*
	 * set properties of node "OXTS Velocity Plot"
	 */

	/*
	 * set properties of node "Show Velocity Plot"
	 */

	/*
	 * set properties of node "OXTS Roll"
	 */

	/*
	 * set properties of node "OXTS Pitch"
	 */

	/*
	 * set properties of node "OXTS Yaw"
	 */

	/*
	 * set properties of node "OXTS forward rate"
	 */

	/*
	 * set properties of node "OXTS leftward rate"
	 */

	/*
	 * set properties of node "OXTS upward rate"
	 */

	/*
	 * set properties of node "OXTS Rotation Plot"
	 */

	/*
	 * set properties of node "Show Rotation Plot"
	 */

	/*
	 * set properties of node "Kitti Pose"
	 */

	/*
	 * set properties of node "Kitti Ego Motion"
	 */

	/*
	 * set properties of node "Kitti Ego Motion FPS Counter"
	 */

	/*
	 * set properties of node "Ego Motion Trajectory"
	 */

	/*
	 * set properties of node "Draw Ego Motion Trajectory"
	 */

	/*
	 * set properties of node "Show Ego Motion Trajectory"
	 */

	/*
	 * set properties of node "Convert to Gray"
	 */

	/*
	 * set properties of node "Optical Motion"
	 */

	/*
	 * set properties of node "Optical Motion FPS Counter"
	 */

	/*
	 * set properties of node "FPS Plot"
	 */

	/*
	 * set properties of node "Show FPS Plot"
	 */

	/*
	 * set properties of node "Ego Motion Quiver"
	 */

	/*
	 * set properties of node "Show Ego Motion Quiver"
	 */

	/*
	 * set properties of node "Optical Motion Quiver"
	 */

	/*
	 * set properties of node "Show Optical Motion Quiver"
	 */


	/* 
	 * connect output port 2 of node "Kitti Reader"
	 * to input port 0 of node "Show Camera 2"
	 */
	try {
		connect_port(node_0,2,node_1,0);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 0 of node \"node_1\" (\"Show Camera 2\") with port 2 of node \"node_0\" (\"Kitti Reader\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 5 of node "Kitti Reader"
	 * to input port 0 of node "OXTS Forward Velocity"
	 */
	try {
		connect_port(node_0,5,node_2,0);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 0 of node \"node_2\" (\"OXTS Forward Velocity\") with port 5 of node \"node_0\" (\"Kitti Reader\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 5 of node "Kitti Reader"
	 * to input port 0 of node "OXTS Leftward Velocity"
	 */
	try {
		connect_port(node_0,5,node_3,0);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 0 of node \"node_3\" (\"OXTS Leftward Velocity\") with port 5 of node \"node_0\" (\"Kitti Reader\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 5 of node "Kitti Reader"
	 * to input port 0 of node "OXTS Upward Velocity"
	 */
	try {
		connect_port(node_0,5,node_4,0);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 0 of node \"node_4\" (\"OXTS Upward Velocity\") with port 5 of node \"node_0\" (\"Kitti Reader\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 0 of node "OXTS Forward Velocity"
	 * to input port 0 of node "OXTS Velocity Plot"
	 */
	try {
		connect_port(node_2,0,node_5,0);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 0 of node \"node_5\" (\"OXTS Velocity Plot\") with port 0 of node \"node_2\" (\"OXTS Forward Velocity\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 0 of node "OXTS Leftward Velocity"
	 * to input port 1 of node "OXTS Velocity Plot"
	 */
	try {
		connect_port(node_3,0,node_5,1);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 1 of node \"node_5\" (\"OXTS Velocity Plot\") with port 0 of node \"node_3\" (\"OXTS Leftward Velocity\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 0 of node "OXTS Upward Velocity"
	 * to input port 2 of node "OXTS Velocity Plot"
	 */
	try {
		connect_port(node_4,0,node_5,2);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 2 of node \"node_5\" (\"OXTS Velocity Plot\") with port 0 of node \"node_4\" (\"OXTS Upward Velocity\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 0 of node "OXTS Velocity Plot"
	 * to input port 0 of node "Show Velocity Plot"
	 */
	try {
		connect_port(node_5,0,node_6,0);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 0 of node \"node_6\" (\"Show Velocity Plot\") with port 0 of node \"node_5\" (\"OXTS Velocity Plot\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 5 of node "Kitti Reader"
	 * to input port 0 of node "OXTS Roll"
	 */
	try {
		connect_port(node_0,5,node_7,0);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 0 of node \"node_7\" (\"OXTS Roll\") with port 5 of node \"node_0\" (\"Kitti Reader\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 5 of node "Kitti Reader"
	 * to input port 0 of node "OXTS Pitch"
	 */
	try {
		connect_port(node_0,5,node_8,0);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 0 of node \"node_8\" (\"OXTS Pitch\") with port 5 of node \"node_0\" (\"Kitti Reader\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 5 of node "Kitti Reader"
	 * to input port 0 of node "OXTS Yaw"
	 */
	try {
		connect_port(node_0,5,node_9,0);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 0 of node \"node_9\" (\"OXTS Yaw\") with port 5 of node \"node_0\" (\"Kitti Reader\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 5 of node "Kitti Reader"
	 * to input port 0 of node "OXTS forward rate"
	 */
	try {
		connect_port(node_0,5,node_10,0);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 0 of node \"node_10\" (\"OXTS forward rate\") with port 5 of node \"node_0\" (\"Kitti Reader\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 5 of node "Kitti Reader"
	 * to input port 0 of node "OXTS leftward rate"
	 */
	try {
		connect_port(node_0,5,node_11,0);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 0 of node \"node_11\" (\"OXTS leftward rate\") with port 5 of node \"node_0\" (\"Kitti Reader\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 5 of node "Kitti Reader"
	 * to input port 0 of node "OXTS upward rate"
	 */
	try {
		connect_port(node_0,5,node_12,0);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 0 of node \"node_12\" (\"OXTS upward rate\") with port 5 of node \"node_0\" (\"Kitti Reader\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 0 of node "OXTS Roll"
	 * to input port 0 of node "OXTS Rotation Plot"
	 */
	try {
		connect_port(node_7,0,node_13,0);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 0 of node \"node_13\" (\"OXTS Rotation Plot\") with port 0 of node \"node_7\" (\"OXTS Roll\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 0 of node "OXTS Pitch"
	 * to input port 1 of node "OXTS Rotation Plot"
	 */
	try {
		connect_port(node_8,0,node_13,1);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 1 of node \"node_13\" (\"OXTS Rotation Plot\") with port 0 of node \"node_8\" (\"OXTS Pitch\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 0 of node "OXTS Yaw"
	 * to input port 2 of node "OXTS Rotation Plot"
	 */
	try {
		connect_port(node_9,0,node_13,2);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 2 of node \"node_13\" (\"OXTS Rotation Plot\") with port 0 of node \"node_9\" (\"OXTS Yaw\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 0 of node "OXTS forward rate"
	 * to input port 3 of node "OXTS Rotation Plot"
	 */
	try {
		connect_port(node_10,0,node_13,3);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 3 of node \"node_13\" (\"OXTS Rotation Plot\") with port 0 of node \"node_10\" (\"OXTS forward rate\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 0 of node "OXTS leftward rate"
	 * to input port 4 of node "OXTS Rotation Plot"
	 */
	try {
		connect_port(node_11,0,node_13,4);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 4 of node \"node_13\" (\"OXTS Rotation Plot\") with port 0 of node \"node_11\" (\"OXTS leftward rate\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 0 of node "OXTS upward rate"
	 * to input port 5 of node "OXTS Rotation Plot"
	 */
	try {
		connect_port(node_12,0,node_13,5);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 5 of node \"node_13\" (\"OXTS Rotation Plot\") with port 0 of node \"node_12\" (\"OXTS upward rate\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 0 of node "OXTS Rotation Plot"
	 * to input port 0 of node "Show Rotation Plot"
	 */
	try {
		connect_port(node_13,0,node_14,0);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 0 of node \"node_14\" (\"Show Rotation Plot\") with port 0 of node \"node_13\" (\"OXTS Rotation Plot\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 5 of node "Kitti Reader"
	 * to input port 0 of node "Kitti Pose"
	 */
	try {
		connect_port(node_0,5,node_15,0);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 0 of node \"node_15\" (\"Kitti Pose\") with port 5 of node \"node_0\" (\"Kitti Reader\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 7 of node "Kitti Reader"
	 * to input port 1 of node "Kitti Pose"
	 */
	try {
		connect_port(node_0,7,node_15,1);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 1 of node \"node_15\" (\"Kitti Pose\") with port 7 of node \"node_0\" (\"Kitti Reader\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 4 of node "Kitti Reader"
	 * to input port 0 of node "Kitti Ego Motion"
	 */
	try {
		connect_port(node_0,4,node_16,0);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 0 of node \"node_16\" (\"Kitti Ego Motion\") with port 4 of node \"node_0\" (\"Kitti Reader\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 0 of node "Kitti Pose"
	 * to input port 1 of node "Kitti Ego Motion"
	 */
	try {
		connect_port(node_15,0,node_16,1);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 1 of node \"node_16\" (\"Kitti Ego Motion\") with port 0 of node \"node_15\" (\"Kitti Pose\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 7 of node "Kitti Reader"
	 * to input port 2 of node "Kitti Ego Motion"
	 */
	try {
		connect_port(node_0,7,node_16,2);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 2 of node \"node_16\" (\"Kitti Ego Motion\") with port 7 of node \"node_0\" (\"Kitti Reader\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 0 of node "Kitti Ego Motion"
	 * to input port 0 of node "Kitti Ego Motion FPS Counter"
	 */
	try {
		connect_port(node_16,0,node_17,0);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 0 of node \"node_17\" (\"Kitti Ego Motion FPS Counter\") with port 0 of node \"node_16\" (\"Kitti Ego Motion\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 0 of node "Kitti Pose"
	 * to input port 0 of node "Ego Motion Trajectory"
	 */
	try {
		connect_port(node_15,0,node_18,0);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 0 of node \"node_18\" (\"Ego Motion Trajectory\") with port 0 of node \"node_15\" (\"Kitti Pose\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 0 of node "Ego Motion Trajectory"
	 * to input port 0 of node "Draw Ego Motion Trajectory"
	 */
	try {
		connect_port(node_18,0,node_19,0);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 0 of node \"node_19\" (\"Draw Ego Motion Trajectory\") with port 0 of node \"node_18\" (\"Ego Motion Trajectory\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 7 of node "Kitti Reader"
	 * to input port 1 of node "Draw Ego Motion Trajectory"
	 */
	try {
		connect_port(node_0,7,node_19,1);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 1 of node \"node_19\" (\"Draw Ego Motion Trajectory\") with port 7 of node \"node_0\" (\"Kitti Reader\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 0 of node "Ego Motion Quiver"
	 * to input port 2 of node "Draw Ego Motion Trajectory"
	 */
	try {
		connect_port(node_26,0,node_19,2);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 2 of node \"node_19\" (\"Draw Ego Motion Trajectory\") with port 0 of node \"node_26\" (\"Ego Motion Quiver\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 0 of node "Draw Ego Motion Trajectory"
	 * to input port 0 of node "Show Ego Motion Trajectory"
	 */
	try {
		connect_port(node_19,0,node_20,0);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 0 of node \"node_20\" (\"Show Ego Motion Trajectory\") with port 0 of node \"node_19\" (\"Draw Ego Motion Trajectory\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 2 of node "Kitti Reader"
	 * to input port 0 of node "Convert to Gray"
	 */
	try {
		connect_port(node_0,2,node_21,0);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 0 of node \"node_21\" (\"Convert to Gray\") with port 2 of node \"node_0\" (\"Kitti Reader\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 0 of node "Convert to Gray"
	 * to input port 0 of node "Optical Motion"
	 */
	try {
		connect_port(node_21,0,node_22,0);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 0 of node \"node_22\" (\"Optical Motion\") with port 0 of node \"node_21\" (\"Convert to Gray\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 0 of node "Kitti Ego Motion"
	 * to input port 1 of node "Optical Motion"
	 */
	try {
		connect_port(node_16,0,node_22,1);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 1 of node \"node_22\" (\"Optical Motion\") with port 0 of node \"node_16\" (\"Kitti Ego Motion\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 0 of node "Optical Motion"
	 * to input port 0 of node "Optical Motion FPS Counter"
	 */
	try {
		connect_port(node_22,0,node_23,0);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 0 of node \"node_23\" (\"Optical Motion FPS Counter\") with port 0 of node \"node_22\" (\"Optical Motion\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 0 of node "Kitti Ego Motion FPS Counter"
	 * to input port 0 of node "FPS Plot"
	 */
	try {
		connect_port(node_17,0,node_24,0);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 0 of node \"node_24\" (\"FPS Plot\") with port 0 of node \"node_17\" (\"Kitti Ego Motion FPS Counter\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 0 of node "Optical Motion FPS Counter"
	 * to input port 1 of node "FPS Plot"
	 */
	try {
		connect_port(node_23,0,node_24,1);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 1 of node \"node_24\" (\"FPS Plot\") with port 0 of node \"node_23\" (\"Optical Motion FPS Counter\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 0 of node "FPS Plot"
	 * to input port 0 of node "Show FPS Plot"
	 */
	try {
		connect_port(node_24,0,node_25,0);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 0 of node \"node_25\" (\"Show FPS Plot\") with port 0 of node \"node_24\" (\"FPS Plot\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 0 of node "Kitti Ego Motion"
	 * to input port 0 of node "Ego Motion Quiver"
	 */
	try {
		connect_port(node_16,0,node_26,0);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 0 of node \"node_26\" (\"Ego Motion Quiver\") with port 0 of node \"node_16\" (\"Kitti Ego Motion\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 2 of node "Kitti Reader"
	 * to input port 1 of node "Ego Motion Quiver"
	 */
	try {
		connect_port(node_0,2,node_26,1);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 1 of node \"node_26\" (\"Ego Motion Quiver\") with port 2 of node \"node_0\" (\"Kitti Reader\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 0 of node "Ego Motion Quiver"
	 * to input port 0 of node "Show Ego Motion Quiver"
	 */
	try {
		connect_port(node_26,0,node_27,0);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 0 of node \"node_27\" (\"Show Ego Motion Quiver\") with port 0 of node \"node_26\" (\"Ego Motion Quiver\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 0 of node "Optical Motion"
	 * to input port 0 of node "Optical Motion Quiver"
	 */
	try {
		connect_port(node_22,0,node_28,0);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 0 of node \"node_28\" (\"Optical Motion Quiver\") with port 0 of node \"node_22\" (\"Optical Motion\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 2 of node "Kitti Reader"
	 * to input port 1 of node "Optical Motion Quiver"
	 */
	try {
		connect_port(node_0,2,node_28,1);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 1 of node \"node_28\" (\"Optical Motion Quiver\") with port 2 of node \"node_0\" (\"Kitti Reader\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 0 of node "Optical Motion Quiver"
	 * to input port 0 of node "Show Optical Motion Quiver"
	 */
	try {
		connect_port(node_28,0,node_29,0);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 0 of node \"node_29\" (\"Show Optical Motion Quiver\") with port 0 of node \"node_28\" (\"Optical Motion Quiver\")\n" << err.what() << ssthrow;
	}

	pipeline.pushNode("Kitti Reader",&node_0);
	pipeline.pushNode("Show Camera 2",&node_1);
	pipeline.pushNode("OXTS Forward Velocity",&node_2);
	pipeline.pushNode("OXTS Leftward Velocity",&node_3);
	pipeline.pushNode("OXTS Upward Velocity",&node_4);
	pipeline.pushNode("OXTS Velocity Plot",&node_5);
	pipeline.pushNode("Show Velocity Plot",&node_6);
	pipeline.pushNode("OXTS Roll",&node_7);
	pipeline.pushNode("OXTS Pitch",&node_8);
	pipeline.pushNode("OXTS Yaw",&node_9);
	pipeline.pushNode("OXTS forward rate",&node_10);
	pipeline.pushNode("OXTS leftward rate",&node_11);
	pipeline.pushNode("OXTS upward rate",&node_12);
	pipeline.pushNode("OXTS Rotation Plot",&node_13);
	pipeline.pushNode("Show Rotation Plot",&node_14);
	pipeline.pushNode("Kitti Pose",&node_15);
	pipeline.pushNode("Kitti Ego Motion",&node_16);
	pipeline.pushNode("Kitti Ego Motion FPS Counter",&node_17);
	pipeline.pushNode("Ego Motion Trajectory",&node_18);
	pipeline.pushNode("Draw Ego Motion Trajectory",&node_19);
	pipeline.pushNode("Show Ego Motion Trajectory",&node_20);
	pipeline.pushNode("Convert to Gray",&node_21);
	pipeline.pushNode("Optical Motion",&node_22);
	pipeline.pushNode("Optical Motion FPS Counter",&node_23);
	pipeline.pushNode("FPS Plot",&node_24);
	pipeline.pushNode("Show FPS Plot",&node_25);
	pipeline.pushNode("Ego Motion Quiver",&node_26);
	pipeline.pushNode("Show Ego Motion Quiver",&node_27);
	pipeline.pushNode("Optical Motion Quiver",&node_28);
	pipeline.pushNode("Show Optical Motion Quiver",&node_29);
}
extern "C" saliency_sandbox::core::INode* new_pipeline() {
	saliency_sandbox::core::Pipeline* pipeline = new saliency_sandbox::core::Pipeline();
	create_pipeline(*pipeline);
	pipeline->initialize();
	return(pipeline);
}
