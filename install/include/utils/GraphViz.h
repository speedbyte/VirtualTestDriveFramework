//
// Created by david on 11.03.17.
//

#include <string>
#include <graphviz/gvc.h>
#include <opencv2/core/core.hpp>

#ifndef GRAPHVIZ_H
#define GRAPHVIZ_H
namespace saliency_sandbox {
    namespace utils {
        class GraphViz {
        private:
            GVC_t* m_context;
            Agraph_t* m_graph;
        public:
            GraphViz(std::string name);

            Agnode_t* node(std::string name);
            Agedge_t* edge(Agnode_t* node0,Agnode_t* node1);

            void set(void* obj, std::string name, std::string value);
            void set(std::string name, std::string value);

            cv::Mat3b mat();
        };
    }
}


#endif //SALIENCYSANDBOX2_0_GRAPHVIZ_H
