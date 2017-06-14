/*  
 *  
 */ 
#include <core/Pipeline.h>
/* 
 * include header for node "Read Data"
 * instance type: "saliency_sandbox::io::_ImageMemoryReader"
 */
#include <io/ImageMemoryReader.h>
/* 
 * include header for node "Convert to LAB"
 * instance type: "saliency_sandbox::utils::_ImageConvert"
 */
#include <utils/Image.h>
/* 
 * include header for node "LAB feature Map"
 * instance type: "saliency_sandbox::utils::_MatrixSplitt"
 */
#include <utils/Matrix.h>
/* 
 * include header for node "Intensity Itti"
 * instance type: "saliency_sandbox::saliency::activation::_Spectral"
 */
#include <saliency/activation/Spectral.h>
/* 
 * include header for node "Green-Red Feature Itti"
 * instance type: "saliency_sandbox::saliency::activation::_Spectral"
 */
#include <saliency/activation/Spectral.h>
/* 
 * include header for node "Blue-Yellow Feature Itti"
 * instance type: "saliency_sandbox::saliency::activation::_Spectral"
 */
#include <saliency/activation/Spectral.h>
/* 
 * include header for node "Merge Itti"
 * instance type: "saliency_sandbox::utils::_MatrixMerge"
 */
#include <utils/Matrix.h>
/* 
 * include header for node "Itti"
 * instance type: "saliency_sandbox::utils::_MatrixSum"
 */
#include <utils/Matrix.h>
/* 
 * include header for node "Itti Heatmap"
 * instance type: "saliency_sandbox::utils::_ImageConvert"
 */
#include <utils/Image.h>
/* 
 * include header for node "Write Data"
 * instance type: "saliency_sandbox::io::ImageMemoryWriter"
 */
#include <io/ImageMemoryWriter.h>

/* 
 * declare instance of node "Read Data"
 */
saliency_sandbox::io::_ImageMemoryReader<uint32_t(640),uint32_t(480),saliency_sandbox::utils::_RGBImage> node_0;
/* 
 * declare instance of node "Convert to LAB"
 */
saliency_sandbox::utils::_ImageConvert<uint32_t(640),uint32_t(480),saliency_sandbox::utils::_RGBImage,saliency_sandbox::utils::_LABImage> node_1;
/* 
 * declare instance of node "LAB feature Map"
 */
saliency_sandbox::utils::_MatrixSplitt<uint32_t(640),uint32_t(480),cv::Vec3f> node_2;
/* 
 * declare instance of node "Intensity Itti"
 */
saliency_sandbox::saliency::activation::_Spectral<uint32_t(640),uint32_t(480)> node_3;
/* 
 * declare instance of node "Green-Red Feature Itti"
 */
saliency_sandbox::saliency::activation::_Spectral<uint32_t(640),uint32_t(480)> node_4;
/* 
 * declare instance of node "Blue-Yellow Feature Itti"
 */
saliency_sandbox::saliency::activation::_Spectral<uint32_t(640),uint32_t(480)> node_5;
/* 
 * declare instance of node "Merge Itti"
 */
saliency_sandbox::utils::_MatrixMerge<uint32_t(640),uint32_t(480),cv::Vec3f> node_6;
/* 
 * declare instance of node "Itti"
 */
saliency_sandbox::utils::_MatrixSum<uint32_t(640),uint32_t(480),cv::Vec3f> node_7;
/* 
 * declare instance of node "Itti Heatmap"
 */
saliency_sandbox::utils::_ImageConvert<uint32_t(640),uint32_t(480),saliency_sandbox::utils::_HeatmapImage,saliency_sandbox::utils::_RGBImage> node_8;
/* 
 * declare instance of node "Write Data"
 */
saliency_sandbox::io::ImageMemoryWriter node_9;

extern "C" void create_pipeline(saliency_sandbox::core::Pipeline& pipeline) {
	/*
	 * set properties of node "Read Data"
	 */

	/*
	 * set properties of node "Convert to LAB"
	 */

	/*
	 * set properties of node "LAB feature Map"
	 */

	/*
	 * set properties of node "Intensity Itti"
	 */

	/*
	 * set properties of node "Green-Red Feature Itti"
	 */

	/*
	 * set properties of node "Blue-Yellow Feature Itti"
	 */

	/*
	 * set properties of node "Merge Itti"
	 */

	/*
	 * set properties of node "Itti"
	 */

	/*
	 * set properties of node "Itti Heatmap"
	 */

	/*
	 * set properties of node "Write Data"
	 */


	/* 
	 * connect output port 0 of node "Read Data"
	 * to input port 0 of node "Convert to LAB"
	 */
	try {
		connect_port(node_0,0,node_1,0);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 0 of node \"node_1\" (\"Convert to LAB\") with port 0 of node \"node_0\" (\"Read Data\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 0 of node "Convert to LAB"
	 * to input port 0 of node "LAB feature Map"
	 */
	try {
		connect_port(node_1,0,node_2,0);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 0 of node \"node_2\" (\"LAB feature Map\") with port 0 of node \"node_1\" (\"Convert to LAB\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 0 of node "LAB feature Map"
	 * to input port 0 of node "Intensity Itti"
	 */
	try {
		connect_port(node_2,0,node_3,0);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 0 of node \"node_3\" (\"Intensity Itti\") with port 0 of node \"node_2\" (\"LAB feature Map\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 1 of node "LAB feature Map"
	 * to input port 0 of node "Green-Red Feature Itti"
	 */
	try {
		connect_port(node_2,1,node_4,0);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 0 of node \"node_4\" (\"Green-Red Feature Itti\") with port 1 of node \"node_2\" (\"LAB feature Map\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 2 of node "LAB feature Map"
	 * to input port 0 of node "Blue-Yellow Feature Itti"
	 */
	try {
		connect_port(node_2,2,node_5,0);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 0 of node \"node_5\" (\"Blue-Yellow Feature Itti\") with port 2 of node \"node_2\" (\"LAB feature Map\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 0 of node "Intensity Itti"
	 * to input port 0 of node "Merge Itti"
	 */
	try {
		connect_port(node_3,0,node_6,0);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 0 of node \"node_6\" (\"Merge Itti\") with port 0 of node \"node_3\" (\"Intensity Itti\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 0 of node "Green-Red Feature Itti"
	 * to input port 1 of node "Merge Itti"
	 */
	try {
		connect_port(node_4,0,node_6,1);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 1 of node \"node_6\" (\"Merge Itti\") with port 0 of node \"node_4\" (\"Green-Red Feature Itti\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 0 of node "Blue-Yellow Feature Itti"
	 * to input port 2 of node "Merge Itti"
	 */
	try {
		connect_port(node_5,0,node_6,2);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 2 of node \"node_6\" (\"Merge Itti\") with port 0 of node \"node_5\" (\"Blue-Yellow Feature Itti\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 0 of node "Merge Itti"
	 * to input port 0 of node "Itti"
	 */
	try {
		connect_port(node_6,0,node_7,0);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 0 of node \"node_7\" (\"Itti\") with port 0 of node \"node_6\" (\"Merge Itti\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 0 of node "Itti"
	 * to input port 0 of node "Itti Heatmap"
	 */
	try {
		connect_port(node_7,0,node_8,0);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 0 of node \"node_8\" (\"Itti Heatmap\") with port 0 of node \"node_7\" (\"Itti\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 0 of node "Itti Heatmap"
	 * to input port 0 of node "Write Data"
	 */
	try {
		connect_port(node_8,0,node_9,0);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 0 of node \"node_9\" (\"Write Data\") with port 0 of node \"node_8\" (\"Itti Heatmap\")\n" << err.what() << ssthrow;
	}

	pipeline.pushNode("Read Data",&node_0);
	pipeline.pushNode("Convert to LAB",&node_1);
	pipeline.pushNode("LAB feature Map",&node_2);
	pipeline.pushNode("Intensity Itti",&node_3);
	pipeline.pushNode("Green-Red Feature Itti",&node_4);
	pipeline.pushNode("Blue-Yellow Feature Itti",&node_5);
	pipeline.pushNode("Merge Itti",&node_6);
	pipeline.pushNode("Itti",&node_7);
	pipeline.pushNode("Itti Heatmap",&node_8);
	pipeline.pushNode("Write Data",&node_9);
}
extern "C" saliency_sandbox::core::INode* new_pipeline() {
	saliency_sandbox::core::Pipeline* pipeline = new saliency_sandbox::core::Pipeline();
	create_pipeline(*pipeline);
	pipeline->initialize();
	return(pipeline);
}
