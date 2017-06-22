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
 * declare instance of node "Kitti Reader"
 */
saliency_sandbox::kitti::KittiReader node_0(std::string("/local/git/PriorityGraphSensors/project/PerceptionSandbox/data/kitti/2011_09_26/2011_09_26_drive_0001_sync/"));
/* 
 * declare instance of node "Show Camera 2"
 */
saliency_sandbox::io::ImageShow node_1(std::string("Kitti Cam 0"));

extern "C" void create_pipeline(saliency_sandbox::core::Pipeline& pipeline) {
	/*
	 * set properties of node "Kitti Reader"
	 */

	/*
	 * set properties of node "Show Camera 2"
	 */


	/* 
	 * connect output port 3 of node "Kitti Reader"
	 * to input port 0 of node "Show Camera 2"
	 */
	try {
		connect_port(node_0,3,node_1,0);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 0 of node \"node_1\" (\"Show Camera 2\") with port 3 of node \"node_0\" (\"Kitti Reader\")\n" << err.what() << ssthrow;
	}

	pipeline.pushNode("Kitti Reader",&node_0);
	pipeline.pushNode("Show Camera 2",&node_1);
}
extern "C" saliency_sandbox::core::INode* new_pipeline() {
	saliency_sandbox::core::Pipeline* pipeline = new saliency_sandbox::core::Pipeline();
	create_pipeline(*pipeline);
	pipeline->initialize();
	return(pipeline);
}
