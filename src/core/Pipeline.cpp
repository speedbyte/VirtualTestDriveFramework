//
// Created by geislerd on 10.02.17.
//

#include <core/Pipeline.h>
#include <graphviz/gvc.h>
#include <highgui.h>
#include <boost/lexical_cast.hpp>
#include <utils/GraphViz.h>

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
            std::chrono::high_resolution_clock::time_point t1, t2;
            long microseconds;

            sserr << sscond(this->m_endNodes.empty()) << "pipeline has no end nodes. process the output ports instead" << ssthrow;

            t1 = std::chrono::high_resolution_clock::now();

            for(int i = 0; i < this->m_endNodes.size(); i++)
                this->m_endNodes[i]->process(time);
            if (time < 0)
                this->m_current++;
            else
                for (; this->m_current < time; this->m_current++);

            t2 = std::chrono::high_resolution_clock::now();

            microseconds = std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count();
            this->m_fps = (1000.0f*1000.0f)/float(microseconds);
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
            boost::unordered::unordered_map<std::string,INode*>::iterator node_iter, node_end;
            boost::unordered::unordered_map<IPort*,Agnode_t*> ports;
            Agnode_t* node;
            Agnode_t* input;
            Agnode_t* output;
            INode* inode;
            IPort* iport;
            std::string iport_name;
            IPort* oport;
            std::string oport_name;

            saliency_sandbox::utils::GraphViz graph(this->name());

            graph.set("nodesep","0.15");
            graph.set("ranksep","0.15");


            for(node_iter = this->m_subsystem.begin();node_iter != node_end; node_iter++) {
                inode = node_iter->second;

                if(!strlen(inode->name()))
                    continue;

                node = graph.node(inode->name());

                graph.set(node,"shape","rectangle");

                for(int i = 0; i < node_iter->second->numInput(); i++) {
                    iport = inode->input(i);

                    if(!strlen(iport->name()))
                        continue;

                    input = graph.node(std::string(inode->name()) + ":" + std::string(iport->name()) + ":in");

                    graph.edge(input,node);

                    graph.set(input,"shape","circle");
                    graph.set(input,"width","0.5");
                    graph.set(input,"style","filled");
                    graph.set(input,"fillcolor","#8EC13A");
                    graph.set(input,"fixedsize","true");
                    graph.set(input,"label",std::string(iport->name())+"\n("+boost::lexical_cast<std::string>(i)+")");
                    graph.set(input,"fontsize","9.0pt");

                    ports[iport] = input;
                }

                for(int i = 0; i < node_iter->second->numOutput(); i++) {
                    oport = inode->output(i);

                    if(!strlen(oport->name()))
                        continue;

                    output = graph.node(std::string(inode->name()) + ":" + std::string(oport->name()) + ":out");

                    graph.edge(node,output);

                    graph.set(output,"shape","circle");
                    graph.set(output,"width","0.5");
                    graph.set(output,"style","filled");
                    graph.set(output,"fillcolor","#C13A8E");
                    graph.set(output,"fixedsize","true");
                    graph.set(output,"label",std::string(oport->name())+"\n("+boost::lexical_cast<std::string>(i)+")");
                    graph.set(output,"fontsize","9.0pt");

                    ports[oport] = output;
                }
            }

            for(node_iter = this->m_subsystem.begin();node_iter != node_end; node_iter++) {
                inode = node_iter->second;

                if(!strlen(inode->name()))
                    continue;

                for(int i = 0; i < node_iter->second->numInput(); i++) {
                    iport = inode->input(i);
                    oport = (IPort*)iport->dependency();

                    if(ports.find(oport) != ports.end())
                        graph.edge(ports[oport],ports[iport]);
                }
            }

            cv::namedWindow("pipeline");
            cv::imshow("pipeline",graph.mat());
        }

        const char *Pipeline::name() {
            return this->m_name.c_str();
        }

        void Pipeline::name(const char *name) {
            this->m_name = std::string(name);
        }

        float Pipeline::fps() {
            return this->m_fps;
        }

    }

}