//
// Created by geislerd on 10.02.17.
//

#include <core/Pipeline.h>
#include <graphviz/gvc.h>
#include <highgui.h>
#include <boost/lexical_cast.hpp>

namespace saliency_sandbox {

    namespace core {

        Pipeline::Pipeline() : m_current(-1) {

        }

        void Pipeline::pushNode(std::string name, INode* node) {
            this->m_subsystem[name] = node;
            node->name(name.c_str());
        }

        void Pipeline::pushNode(INode* node) {
            this->m_subsystem[Utils::uniqueName()] = node;
        }

        INode* Pipeline::removeNode(std::string name) {
            INode* node;

            node = this->m_subsystem[name];
            this->m_subsystem.erase(name);
            return node;
        }

        void Pipeline::initialize() {
            INode* node;
            IPort* port;
            boost::unordered::unordered_map<std::string,INode*>::iterator node_iter, node_end;
            boost::unordered::unordered_set<IPort*> outputs;
            boost::unordered::unordered_set<IPort*>::iterator out_iter, out_end;

            this->m_endNodes.clear();
            this->m_inputs.clear();
            this->m_outputs.clear();

            node_end = this->m_subsystem.end();

            for(node_iter = this->m_subsystem.begin();node_iter != node_end; node_iter++) {
                node = node_iter->second;
                if(node->numOutput() == 0)
                    //standalone node
                    this->m_endNodes.push_back(node);
                else
                    for(int i = 0; i < node->numOutput(); i++) {
                        port = node->output(i);
                        outputs.insert(port);
                    }
            }

            for(node_iter = this->m_subsystem.begin();node_iter != node_end; node_iter++) {
                node = node_iter->second;
                for(int i = 0; i < node->numInput(); i++) {
                    port = node->input(i);
                    if(port->connect() == nullptr)
                        //unconnected input port
                        this->m_inputs.push_back(port);
                    else
                        outputs.erase(port->connect());
                }
            }

            out_end = outputs.end();

            for(out_iter = outputs.begin(); out_iter != out_end; out_iter++)
                this->m_outputs.push_back(*out_iter);
        }

        void Pipeline::process(time_t time) {
            sserr << sscond(this->m_endNodes.empty()) << "pipeline has no end nodes. process the output ports instead" << ssthrow;
            for(int i = 0; i < this->m_endNodes.size(); i++)
                this->m_endNodes[i]->process(time);
            if (time < 0)
                this->m_current++;
            else
                for (; this->m_current < time; this->m_current++);
        }

        time_t Pipeline::time() {
            return this->m_current;
        }

        uint32_t Pipeline::numInput() {
            return (uint32_t) this->m_inputs.size();
        }

        uint32_t Pipeline::numOutput() {
            return (uint32_t) this->m_outputs.size();
        }

        IPort* Pipeline::input(uint32_t index) {
            return this->m_inputs[index];
        }

        IPort* Pipeline::output(uint32_t index) {
            return this->m_outputs[index];
        }

        PropertyMap* Pipeline::properties() {
            return &(this->m_properties);
        }

        bool Pipeline::eof() {
            for (int i = 0; i < this->m_endNodes.size(); i++)
                if (this->m_endNodes[i]->eof())
                    return true;
            for (int i = 0; i < this->m_outputs.size(); i++)
                if (this->m_outputs[i]->eof())
                    return true;
            return false;
        }

        void Pipeline::show() {
            GVC_t* gvc;
            Agraph_t* g;
            boost::unordered::unordered_map<std::string,INode*>::iterator node_iter, node_end;
            boost::unordered::unordered_map<INode*,Agnode_t*> nodes;
            boost::unordered::unordered_map<IPort*,Agnode_t*> ports;
            char* img;
            unsigned int img_size;
            cv::Mat1b raw_mat;
            cv::Mat3b img_mat;
            Agnode_t* node;
            Agnode_t* input;
            Agedge_t* node_input_edge;
            Agnode_t* output;
            Agedge_t* node_output_edge;
            INode* inode;
            IPort* iport;
            std::string iport_name;
            IPort* oport;
            std::string oport_name;
            Agedge_t* port_port_edge;

            gvc = gvContext();

            g = agopen((char*)this->name(), Agdirected, 0);
            agsafeset(g,"nodesep","0.15","");
            agsafeset(g,"ranksep","0.15","");


            for(node_iter = this->m_subsystem.begin();node_iter != node_end; node_iter++) {
                inode = node_iter->second;
                node = agnode(g,(char*)inode->name(),1);
                nodes[node_iter->second] = node;

                agsafeset(node,"shape","rectangle","");

                for(int i = 0; i < node_iter->second->numInput(); i++) {
                    iport = inode->input(i);
                    iport_name = std::string(inode->name()) + ":" + std::string(iport->name());
                    input = agnode(g,(char*)iport_name.c_str(),1);
                    node_input_edge = agedge(g,input,node,"",1);
                    agsafeset(input,"shape","circle","");
                    agsafeset(input,"width","0.5","");
                    agsafeset(input,"style","filled","");
                    agsafeset(input,"fillcolor","#8EC13A","");
                    agsafeset(input,"fixedsize","true","");
                    agsafeset(input,"label",(char*)(std::string(iport->name())+"\n("+boost::lexical_cast<std::string>(i)+")").c_str(),"");
                    agsafeset(input,"fontsize","9.0pt","");

                    ports[iport] = input;
                }

                for(int i = 0; i < node_iter->second->numOutput(); i++) {
                    oport = inode->output(i);
                    oport_name = std::string(inode->name()) + ":" + std::string(oport->name());
                    output = agnode(g,(char*)oport_name.c_str(),1);
                    node_output_edge = agedge(g,node,output,"",1);
                    agsafeset(output,"shape","circle","");
                    agsafeset(output,"shape","circle","");
                    agsafeset(output,"width","0.5","");
                    agsafeset(output,"style","filled","");
                    agsafeset(output,"fillcolor","#C13A8E","");
                    agsafeset(output,"fixedsize","true","");
                    agsafeset(output,"label",(char*)(std::string(oport->name())+"\n("+boost::lexical_cast<std::string>(i)+")").c_str(),"");
                    agsafeset(output,"fontsize","9.0pt","");

                    ports[oport] = output;
                }
            }

            for(node_iter = this->m_subsystem.begin();node_iter != node_end; node_iter++) {
                inode = node_iter->second;

                for(int i = 0; i < node_iter->second->numInput(); i++) {
                    iport = inode->input(i);
                    oport = (IPort*)iport->dependency();

                    if(ports.find(oport) != ports.end()) {
                        input = ports[iport];
                        output = ports[oport];
                        port_port_edge = agedge(g,output,input,"",1);
                    }
                }
            }

            gvLayout(gvc,g,"dot");
            gvRenderData(gvc,g,"png",&img,&img_size);

            raw_mat = cv::Mat1b(1,(int)img_size,(uchar*)img);
            img_mat = cv::imdecode(raw_mat,1);

            cv::namedWindow("pipeline");
            cv::imshow("pipeline",img_mat);
        }

        const char *Pipeline::name() {
            return this->m_name.c_str();
        }

        void Pipeline::name(const char *name) {
            this->m_name = std::string(name);
        }

    }

}