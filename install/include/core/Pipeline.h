//
// Created by geislerd on 10.02.17.
//

#ifndef PIPELINE_H
#define PIPELINE_H

#include <core/Core.h>

namespace saliency_sandbox {

    namespace core {

        class Pipeline : public INode {
        private:
            boost::unordered::unordered_map<std::string,INode*> m_subsystem;
            std::vector<IPort*> m_inputs;
            std::vector<IPort*> m_outputs;
            std::vector<INode*> m_endNodes;
            PropertyMap m_properties;
            time_t m_current;
            std::string m_name;
            float m_fps;

        public:
            Pipeline();

            void pushNode(std::string name, INode* node);

            void pushNode(INode* node);

            INode* removeNode(std::string name);

            void initialize();

            void process(time_t time) override;

            time_t time() override;

            uint32_t numInput() override;

            uint32_t numOutput() override;

            IPort *input(uint32_t index) override;

            IPort *output(uint32_t index) override;

            PropertyMap *properties() override;

            bool eof() override;

            const char *name() override;

            void name(const char *name) override;

            float fps() override;

            void show();
        };

    }

}
#endif //PIPELINE_H
