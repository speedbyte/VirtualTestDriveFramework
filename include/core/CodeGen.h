//
// Created by geislerd on 08.03.17.
//


// protobuf
#include <src/core/node.pb.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/text_format.h>

// boost
#include <boost/filesystem.hpp>

#include <sstream>

#ifndef CODE_GEN_H
#define CODE_GEN_H

namespace saliency_sandbox {

    namespace core {

        class CodeGen {
        private:
            generated::Pipeline m_pipeline;
            int m_node_idx;
            std::string m_header;
            std::string m_classname;

            std::vector<const generated::Pipeline_Node_Argument*> m_template_arguments;
            std::vector<const generated::Pipeline_Node_Argument*> m_constructor_arguments;
            std::vector<const generated::Pipeline_Node_Input*> m_inputs;

            std::stringstream m_include_ss;
            std::stringstream m_declare_ss;
            std::stringstream m_init_ss;
            std::stringstream m_connect_ss;

            const generated::Pipeline& pipeline();

            const int nodeIdx();

            generated::Pipeline_Node& node();

            std::string header();

            std::string classname();

            void pushTemplateArgument(const generated::Pipeline_Node_Argument *argument);

            void pushConstructorArgument(const generated::Pipeline_Node_Argument *argument);

            void pushInput(const generated::Pipeline_Node_Input *input);

            void generate();

        public:
            CodeGen();

            class SetPipeline {
            private:
                generated::Pipeline& m_pipeline;
            public:
                SetPipeline(generated::Pipeline &m_pipeline);

                generated::Pipeline& pipeline() const;
            };

            class SetNode {
            private:
                int m_node_idx;
            public:
                SetNode(int node_idx);

                int nodeIdx() const;
            };

            class SetHeader {
            private:
                std::string m_header;
            public:
                SetHeader(std::string header);

                std::string header() const;
            };

            class SetClass {
            private:
                std::string m_classname;
            public:
                SetClass(std::string classname);

                std::string classname() const;
            };

            class SetInput {
            private:
                std::string m_name;
            public:
                SetInput(std::string name, uint32_t idx);

                std::string name() const;
            };

            class SetNoInput { };

            class SetTemplateArgument {
            private:
                std::string m_name;
                generated::Pipeline_Node_Argument_Type m_type;
                bool m_optional;
                std::string m_default;
            public:
                SetTemplateArgument(std::string name, generated::Pipeline_Node_Argument_Type type);

                SetTemplateArgument(std::string name, generated::Pipeline_Node_Argument_Type type, std::string _default);

                std::string name() const;

                generated::Pipeline_Node_Argument_Type type() const;

                bool optional() const;

                std::string _default() const;

            };

            class SetConstructorArgument {
            private:
                std::string m_name;
                generated::Pipeline_Node_Argument_Type m_type;
                bool m_optional;
                std::string m_default;
            public:
                SetConstructorArgument(std::string name, generated::Pipeline_Node_Argument_Type type);

                SetConstructorArgument(std::string name, generated::Pipeline_Node_Argument_Type type, std::string _default);

                std::string name() const;

                generated::Pipeline_Node_Argument_Type type() const;

                bool optional() const;

                std::string _default() const;
            };

            class SetProperty {
            private:
                std::string m_name;
                generated::Pipeline_Node_Property_Type m_type;
            public:
                SetProperty(std::string name, generated::Pipeline_Node_Property_Type type);

                std::string name() const;

                generated::Pipeline_Node_Property_Type type() const;
            };

            CodeGen& operator<<(const SetPipeline& pipeline);

            CodeGen& operator<<(const SetNode& node);

            CodeGen& operator<<(const SetHeader& header);

            CodeGen& operator<<(const SetClass& clazz);

            CodeGen& operator<<(const SetTemplateArgument& templateArgument);

            CodeGen& operator<<(const SetConstructorArgument& constructorArgument);

            CodeGen& operator<<(const SetInput& input);

            CodeGen& operator<<(const SetNoInput& input);

            CodeGen& operator<<(const SetProperty& sproperty);

            std::string cpp();

            static std::string cpp(boost::filesystem::path config);

            static std::string cpp(generated::Pipeline& pipeline);
        };
    }
}

#endif //CODE_GEN_H
