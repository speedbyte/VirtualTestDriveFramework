/*  
 *  
 */ 
#include <core/Pipeline.h>
/* 
 * include header for node "Video Reader"
 * instance type: "saliency_sandbox::adtf::RGBImageReader"
 */
#include <adtf/io/ImageReader.h>
/* 
 * include header for node "RGB feature Map"
 * instance type: "saliency_sandbox::utils::_MatrixSplitt"
 */
#include <utils/Matrix.h>
/* 
 * include header for node "Convert channel R to Heatmap"
 * instance type: "saliency_sandbox::utils::_ImageConvert"
 */
#include <utils/Image.h>
/* 
 * include header for node "Convert channel G to Heatmap"
 * instance type: "saliency_sandbox::utils::_ImageConvert"
 */
#include <utils/Image.h>
/* 
 * include header for node "Convert channel B to Heatmap"
 * instance type: "saliency_sandbox::utils::_ImageConvert"
 */
#include <utils/Image.h>
/* 
 * include header for node "Red Spectral Whitening"
 * instance type: "saliency_sandbox::saliency::activation::_Spectral"
 */
#include <saliency/activation/Spectral.h>
/* 
 * include header for node "Green Spectral Whitening"
 * instance type: "saliency_sandbox::saliency::activation::_Spectral"
 */
#include <saliency/activation/Spectral.h>
/* 
 * include header for node "Blue Spectral Whitening"
 * instance type: "saliency_sandbox::saliency::activation::_Spectral"
 */
#include <saliency/activation/Spectral.h>
/* 
 * include header for node "Merge Spectral Whitening Activations"
 * instance type: "saliency_sandbox::utils::_MatrixMerge"
 */
#include <utils/Matrix.h>
/* 
 * include header for node "Spectral Whitening"
 * instance type: "saliency_sandbox::utils::_MatrixSum"
 */
#include <utils/Matrix.h>
/* 
 * include header for node "Video Reader FPS Counter"
 * instance type: "saliency_sandbox::utils::FPSCounter"
 */
#include <utils/FPSCounter.h>
/* 
 * include header for node "Red Spectral Whitening FPS Counter"
 * instance type: "saliency_sandbox::utils::FPSCounter"
 */
#include <utils/FPSCounter.h>
/* 
 * include header for node "Green Spectral Whitening FPS Counter"
 * instance type: "saliency_sandbox::utils::FPSCounter"
 */
#include <utils/FPSCounter.h>
/* 
 * include header for node "Blue Spectral Whitening FPS Counter"
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
 * include header for node "Spectral Whitening Heatmap"
 * instance type: "saliency_sandbox::utils::_ImageConvert"
 */
#include <utils/Image.h>
/* 
 * include header for node "Show Spectral Whitening Heatmap"
 * instance type: "saliency_sandbox::io::ImageShow"
 */
#include <io/ImageShow.h>
/* 
 * include header for node "Show Image"
 * instance type: "saliency_sandbox::io::ImageShow"
 */
#include <io/ImageShow.h>

/* 
 * declare instance of node "Video Reader"
 */
saliency_sandbox::adtf::RGBImageReader<uint32_t(1242),uint32_t(375)> node_0;
/* 
 * declare instance of node "RGB feature Map"
 */
saliency_sandbox::utils::_MatrixSplitt<uint32_t(1242),uint32_t(375),cv::Vec3b> node_1;
/* 
 * declare instance of node "Convert channel R to Heatmap"
 */
saliency_sandbox::utils::_ImageConvert<uint32_t(1242),uint32_t(375),saliency_sandbox::utils::_IntensityImage,saliency_sandbox::utils::_HeatmapImage> node_2;
/* 
 * declare instance of node "Convert channel G to Heatmap"
 */
saliency_sandbox::utils::_ImageConvert<uint32_t(1242),uint32_t(375),saliency_sandbox::utils::_IntensityImage,saliency_sandbox::utils::_HeatmapImage> node_3;
/* 
 * declare instance of node "Convert channel B to Heatmap"
 */
saliency_sandbox::utils::_ImageConvert<uint32_t(1242),uint32_t(375),saliency_sandbox::utils::_IntensityImage,saliency_sandbox::utils::_HeatmapImage> node_4;
/* 
 * declare instance of node "Red Spectral Whitening"
 */
saliency_sandbox::saliency::activation::_Spectral<uint32_t(1242),uint32_t(375)> node_5;
/* 
 * declare instance of node "Green Spectral Whitening"
 */
saliency_sandbox::saliency::activation::_Spectral<uint32_t(1242),uint32_t(375)> node_6;
/* 
 * declare instance of node "Blue Spectral Whitening"
 */
saliency_sandbox::saliency::activation::_Spectral<uint32_t(1242),uint32_t(375)> node_7;
/* 
 * declare instance of node "Merge Spectral Whitening Activations"
 */
saliency_sandbox::utils::_MatrixMerge<uint32_t(1242),uint32_t(375),cv::Vec3f> node_8;
/* 
 * declare instance of node "Spectral Whitening"
 */
saliency_sandbox::utils::_MatrixSum<uint32_t(1242),uint32_t(375),cv::Vec3f> node_9;
/* 
 * declare instance of node "Video Reader FPS Counter"
 */
saliency_sandbox::utils::FPSCounter node_10;
/* 
 * declare instance of node "Red Spectral Whitening FPS Counter"
 */
saliency_sandbox::utils::FPSCounter node_11;
/* 
 * declare instance of node "Green Spectral Whitening FPS Counter"
 */
saliency_sandbox::utils::FPSCounter node_12;
/* 
 * declare instance of node "Blue Spectral Whitening FPS Counter"
 */
saliency_sandbox::utils::FPSCounter node_13;
/* 
 * declare instance of node "FPS Plot"
 */
saliency_sandbox::plot::Plot node_14(std::string("Video Reader"),std::string("Red Spectral Whitening"),std::string("Green Spectral Whitening"),std::string("Blue Spectral Whitening"));
/* 
 * declare instance of node "Show FPS Plot"
 */
saliency_sandbox::io::ImageShow node_15(std::string("FPS"));
/* 
 * declare instance of node "Spectral Whitening Heatmap"
 */
saliency_sandbox::utils::_ImageConvert<uint32_t(1242),uint32_t(375),saliency_sandbox::utils::_HeatmapImage,saliency_sandbox::utils::_RGBImage> node_16;
/* 
 * declare instance of node "Show Spectral Whitening Heatmap"
 */
saliency_sandbox::io::ImageShow node_17(std::string("Spectral Whitening"));
/* 
 * declare instance of node "Show Image"
 */
saliency_sandbox::io::ImageShow node_18(std::string("Raw Image"));

extern "C" void create_pipeline(saliency_sandbox::core::Pipeline& pipeline) {
	/*
	 * set properties of node "Video Reader"
	 */

	/*
	 * set properties of node "RGB feature Map"
	 */

	/*
	 * set properties of node "Convert channel R to Heatmap"
	 */

	/*
	 * set properties of node "Convert channel G to Heatmap"
	 */

	/*
	 * set properties of node "Convert channel B to Heatmap"
	 */

	/*
	 * set properties of node "Red Spectral Whitening"
	 */

	/*
	 * set properties of node "Green Spectral Whitening"
	 */

	/*
	 * set properties of node "Blue Spectral Whitening"
	 */

	/*
	 * set properties of node "Merge Spectral Whitening Activations"
	 */

	/*
	 * set properties of node "Spectral Whitening"
	 */

	/*
	 * set properties of node "Video Reader FPS Counter"
	 */

	/*
	 * set properties of node "Red Spectral Whitening FPS Counter"
	 */

	/*
	 * set properties of node "Green Spectral Whitening FPS Counter"
	 */

	/*
	 * set properties of node "Blue Spectral Whitening FPS Counter"
	 */

	/*
	 * set properties of node "FPS Plot"
	 */

	/*
	 * set properties of node "Show FPS Plot"
	 */

	/*
	 * set properties of node "Spectral Whitening Heatmap"
	 */

	/*
	 * set properties of node "Show Spectral Whitening Heatmap"
	 */

	/*
	 * set properties of node "Show Image"
	 */


	/* 
	 * connect output port 0 of node "Video Reader"
	 * to input port 0 of node "RGB feature Map"
	 */
	try {
		connect_port(node_0,0,node_1,0);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 0 of node \"node_1\" (\"RGB feature Map\") with port 0 of node \"node_0\" (\"Video Reader\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 0 of node "RGB feature Map"
	 * to input port 0 of node "Convert channel R to Heatmap"
	 */
	try {
		connect_port(node_1,0,node_2,0);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 0 of node \"node_2\" (\"Convert channel R to Heatmap\") with port 0 of node \"node_1\" (\"RGB feature Map\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 1 of node "RGB feature Map"
	 * to input port 0 of node "Convert channel G to Heatmap"
	 */
	try {
		connect_port(node_1,1,node_3,0);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 0 of node \"node_3\" (\"Convert channel G to Heatmap\") with port 1 of node \"node_1\" (\"RGB feature Map\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 2 of node "RGB feature Map"
	 * to input port 0 of node "Convert channel B to Heatmap"
	 */
	try {
		connect_port(node_1,2,node_4,0);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 0 of node \"node_4\" (\"Convert channel B to Heatmap\") with port 2 of node \"node_1\" (\"RGB feature Map\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 0 of node "Convert channel R to Heatmap"
	 * to input port 0 of node "Red Spectral Whitening"
	 */
	try {
		connect_port(node_2,0,node_5,0);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 0 of node \"node_5\" (\"Red Spectral Whitening\") with port 0 of node \"node_2\" (\"Convert channel R to Heatmap\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 0 of node "Convert channel G to Heatmap"
	 * to input port 0 of node "Green Spectral Whitening"
	 */
	try {
		connect_port(node_3,0,node_6,0);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 0 of node \"node_6\" (\"Green Spectral Whitening\") with port 0 of node \"node_3\" (\"Convert channel G to Heatmap\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 0 of node "Convert channel B to Heatmap"
	 * to input port 0 of node "Blue Spectral Whitening"
	 */
	try {
		connect_port(node_4,0,node_7,0);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 0 of node \"node_7\" (\"Blue Spectral Whitening\") with port 0 of node \"node_4\" (\"Convert channel B to Heatmap\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 0 of node "Red Spectral Whitening"
	 * to input port 0 of node "Merge Spectral Whitening Activations"
	 */
	try {
		connect_port(node_5,0,node_8,0);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 0 of node \"node_8\" (\"Merge Spectral Whitening Activations\") with port 0 of node \"node_5\" (\"Red Spectral Whitening\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 0 of node "Green Spectral Whitening"
	 * to input port 1 of node "Merge Spectral Whitening Activations"
	 */
	try {
		connect_port(node_6,0,node_8,1);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 1 of node \"node_8\" (\"Merge Spectral Whitening Activations\") with port 0 of node \"node_6\" (\"Green Spectral Whitening\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 0 of node "Blue Spectral Whitening"
	 * to input port 2 of node "Merge Spectral Whitening Activations"
	 */
	try {
		connect_port(node_7,0,node_8,2);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 2 of node \"node_8\" (\"Merge Spectral Whitening Activations\") with port 0 of node \"node_7\" (\"Blue Spectral Whitening\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 0 of node "Merge Spectral Whitening Activations"
	 * to input port 0 of node "Spectral Whitening"
	 */
	try {
		connect_port(node_8,0,node_9,0);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 0 of node \"node_9\" (\"Spectral Whitening\") with port 0 of node \"node_8\" (\"Merge Spectral Whitening Activations\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 0 of node "Video Reader"
	 * to input port 0 of node "Video Reader FPS Counter"
	 */
	try {
		connect_port(node_0,0,node_10,0);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 0 of node \"node_10\" (\"Video Reader FPS Counter\") with port 0 of node \"node_0\" (\"Video Reader\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 0 of node "Red Spectral Whitening"
	 * to input port 0 of node "Red Spectral Whitening FPS Counter"
	 */
	try {
		connect_port(node_5,0,node_11,0);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 0 of node \"node_11\" (\"Red Spectral Whitening FPS Counter\") with port 0 of node \"node_5\" (\"Red Spectral Whitening\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 0 of node "Green Spectral Whitening"
	 * to input port 0 of node "Green Spectral Whitening FPS Counter"
	 */
	try {
		connect_port(node_6,0,node_12,0);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 0 of node \"node_12\" (\"Green Spectral Whitening FPS Counter\") with port 0 of node \"node_6\" (\"Green Spectral Whitening\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 0 of node "Blue Spectral Whitening"
	 * to input port 0 of node "Blue Spectral Whitening FPS Counter"
	 */
	try {
		connect_port(node_7,0,node_13,0);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 0 of node \"node_13\" (\"Blue Spectral Whitening FPS Counter\") with port 0 of node \"node_7\" (\"Blue Spectral Whitening\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 0 of node "Video Reader FPS Counter"
	 * to input port 0 of node "FPS Plot"
	 */
	try {
		connect_port(node_10,0,node_14,0);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 0 of node \"node_14\" (\"FPS Plot\") with port 0 of node \"node_10\" (\"Video Reader FPS Counter\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 0 of node "Red Spectral Whitening FPS Counter"
	 * to input port 1 of node "FPS Plot"
	 */
	try {
		connect_port(node_11,0,node_14,1);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 1 of node \"node_14\" (\"FPS Plot\") with port 0 of node \"node_11\" (\"Red Spectral Whitening FPS Counter\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 0 of node "Green Spectral Whitening FPS Counter"
	 * to input port 2 of node "FPS Plot"
	 */
	try {
		connect_port(node_12,0,node_14,2);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 2 of node \"node_14\" (\"FPS Plot\") with port 0 of node \"node_12\" (\"Green Spectral Whitening FPS Counter\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 0 of node "Blue Spectral Whitening FPS Counter"
	 * to input port 3 of node "FPS Plot"
	 */
	try {
		connect_port(node_13,0,node_14,3);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 3 of node \"node_14\" (\"FPS Plot\") with port 0 of node \"node_13\" (\"Blue Spectral Whitening FPS Counter\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 0 of node "FPS Plot"
	 * to input port 0 of node "Show FPS Plot"
	 */
	try {
		connect_port(node_14,0,node_15,0);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 0 of node \"node_15\" (\"Show FPS Plot\") with port 0 of node \"node_14\" (\"FPS Plot\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 0 of node "Spectral Whitening"
	 * to input port 0 of node "Spectral Whitening Heatmap"
	 */
	try {
		connect_port(node_9,0,node_16,0);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 0 of node \"node_16\" (\"Spectral Whitening Heatmap\") with port 0 of node \"node_9\" (\"Spectral Whitening\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 0 of node "Spectral Whitening Heatmap"
	 * to input port 0 of node "Show Spectral Whitening Heatmap"
	 */
	try {
		connect_port(node_16,0,node_17,0);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 0 of node \"node_17\" (\"Show Spectral Whitening Heatmap\") with port 0 of node \"node_16\" (\"Spectral Whitening Heatmap\")\n" << err.what() << ssthrow;
	}
	/* 
	 * connect output port 0 of node "Video Reader"
	 * to input port 0 of node "Show Image"
	 */
	try {
		connect_port(node_0,0,node_18,0);
	} catch(std::runtime_error& err) {
		sserr << "error while connecting port 0 of node \"node_18\" (\"Show Image\") with port 0 of node \"node_0\" (\"Video Reader\")\n" << err.what() << ssthrow;
	}

	pipeline.pushNode("Video Reader",&node_0);
	pipeline.pushNode("RGB feature Map",&node_1);
	pipeline.pushNode("Convert channel R to Heatmap",&node_2);
	pipeline.pushNode("Convert channel G to Heatmap",&node_3);
	pipeline.pushNode("Convert channel B to Heatmap",&node_4);
	pipeline.pushNode("Red Spectral Whitening",&node_5);
	pipeline.pushNode("Green Spectral Whitening",&node_6);
	pipeline.pushNode("Blue Spectral Whitening",&node_7);
	pipeline.pushNode("Merge Spectral Whitening Activations",&node_8);
	pipeline.pushNode("Spectral Whitening",&node_9);
	pipeline.pushNode("Video Reader FPS Counter",&node_10);
	pipeline.pushNode("Red Spectral Whitening FPS Counter",&node_11);
	pipeline.pushNode("Green Spectral Whitening FPS Counter",&node_12);
	pipeline.pushNode("Blue Spectral Whitening FPS Counter",&node_13);
	pipeline.pushNode("FPS Plot",&node_14);
	pipeline.pushNode("Show FPS Plot",&node_15);
	pipeline.pushNode("Spectral Whitening Heatmap",&node_16);
	pipeline.pushNode("Show Spectral Whitening Heatmap",&node_17);
	pipeline.pushNode("Show Image",&node_18);
}
extern "C" saliency_sandbox::core::INode* new_pipeline() {
	saliency_sandbox::core::Pipeline* pipeline = new saliency_sandbox::core::Pipeline();
	create_pipeline(*pipeline);
	pipeline->initialize();
	return(pipeline);
}
