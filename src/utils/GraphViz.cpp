//
// Created by david on 11.03.17.
//

#include <utils/GraphViz.h>
#include <highgui.h>
#include <opencv2/imgcodecs.hpp>

namespace saliency_sandbox {
    namespace utils {
        GraphViz::GraphViz(std::string name) :
                m_context(gvContext()),
                m_graph(agopen((char*)name.c_str(), Agdirected, 0)) {

        }


        Agnode_t* GraphViz::node(std::string name) {
            return agnode(this->m_graph,(char*)name.c_str(),1);
        }

        Agedge_t* GraphViz::edge(Agnode_t* node0,Agnode_t* node1) {
            return agedge(this->m_graph,node0,node1,(char*)"",1);
        }

        void GraphViz::set(void* obj, std::string name, std::string value) {
            agsafeset(obj,(char*)name.c_str(),(char*)value.c_str(),(char*)"");
        }

        void GraphViz::set(std::string name, std::string value) {
            this->set(this->m_graph,name,value);
        }

        cv::Mat3b GraphViz::mat() {
            char* img;
            unsigned int img_size;
            cv::Mat1b raw_mat;
            cv::Mat3b img_mat;

            gvLayout(this->m_context,this->m_graph,"dot");
            gvRenderData(this->m_context,this->m_graph,"png",&img,&img_size);

            raw_mat = cv::Mat1b(1,(int)img_size,(uchar*)img);
            return cv::imdecode(raw_mat,1);
        }
    }
}
