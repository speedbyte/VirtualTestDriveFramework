//
// Created by geislerd on 08.03.17.
//


#include <iomanip>
#include <utils/Error.h>
#include <ostream>
#include <src/core/node.pb.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/text_format.h>
#include <config.h>

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
            std::stringstream m_connect_ss;

            const generated::Pipeline& pipeline() {
                return this->m_pipeline;
            }

            const int nodeIdx() {
                return this->m_node_idx;
            }

            generated::Pipeline_Node& node() {
                sserr << sscond(this->nodeIdx() < 0 || this->nodeIdx() >= this->pipeline().node_size()) << "node not found with index: " << this->nodeIdx() << ssthrow;
                return (generated::Pipeline_Node&)this->pipeline().node(this->nodeIdx());
            }

            std::string header() {
                return this->m_header;
            }

            std::string classname() {
                return this->m_classname;
            }

            void pushTemplateArgument(const generated::Pipeline_Node_Argument *argument) {
                this->m_template_arguments.push_back(argument);
            }

            void pushConstructorArgument(const generated::Pipeline_Node_Argument *argument) {
                this->m_constructor_arguments.push_back(argument);
            }

            void pushInput(const generated::Pipeline_Node_Input *input) {
                this->m_inputs.push_back(input);
            }

            void generate() {

                this->m_include_ss << "/* " << std::endl;
                this->m_include_ss << " * include header for node \"" << this->node().name().c_str() << "\"" << std::endl;
                this->m_include_ss << " * instance type: \"" << this->m_classname.c_str() << "\"" << std::endl;
                this->m_include_ss << " */" << std::endl;
                this->m_include_ss << "#include <" << this->m_header.c_str() << ".h>" << std::endl;

                this->m_declare_ss << "/* " << std::endl;
                this->m_declare_ss << " * declare instance of node \"" << this->node().name().c_str() << "\"" << std::endl;
                this->m_declare_ss << " */" << std::endl;
                this->m_declare_ss << this->m_classname;
                if(this->m_template_arguments.size() > 0) {
                    this->m_declare_ss << "<";
                    for(int i = 0; i < this->m_template_arguments.size(); i++) {
                        if(i > 0)
                            this->m_declare_ss << ",";
                        switch(this->m_template_arguments.at(i)->type()) {
                            case generated::Pipeline_Node_Argument_Type_pb_bool:
                                this->m_declare_ss << "bool(" << this->m_template_arguments.at(i)->value().c_str() << ")";
                                break;
                            case generated::Pipeline_Node_Argument_Type_pb_int8:
                                this->m_declare_ss << "int8_t(" << this->m_template_arguments.at(i)->value().c_str() << ")";
                                break;
                            case generated::Pipeline_Node_Argument_Type_pb_int16:
                                this->m_declare_ss << "int16_t(" << this->m_template_arguments.at(i)->value().c_str() << ")";
                                break;
                            case generated::Pipeline_Node_Argument_Type_pb_int32:
                                this->m_declare_ss << "int32_t(" << this->m_template_arguments.at(i)->value().c_str() << ")";
                                break;
                            case generated::Pipeline_Node_Argument_Type_pb_int64:
                                this->m_declare_ss << "int64_t(" << this->m_template_arguments.at(i)->value().c_str() << ")";
                                break;
                            case generated::Pipeline_Node_Argument_Type_pb_uint8:
                                this->m_declare_ss << "uint8_t(" << this->m_template_arguments.at(i)->value().c_str() << ")";
                                break;
                            case generated::Pipeline_Node_Argument_Type_pb_uint16:
                                this->m_declare_ss << "uint16_t(" << this->m_template_arguments.at(i)->value().c_str() << ")";
                                break;
                            case generated::Pipeline_Node_Argument_Type_pb_uint32:
                                this->m_declare_ss << "uint32_t(" << this->m_template_arguments.at(i)->value().c_str() << ")";
                                break;
                            case generated::Pipeline_Node_Argument_Type_pb_uint64:
                                this->m_declare_ss << "uint64_t(" << this->m_template_arguments.at(i)->value().c_str() << ")";
                                break;
                            case generated::Pipeline_Node_Argument_Type_pb_float32:
                                this->m_declare_ss << "float(" << this->m_template_arguments.at(i)->value().c_str() << ")";
                                break;
                            case generated::Pipeline_Node_Argument_Type_pb_float64:
                                this->m_declare_ss << "double(" << this->m_template_arguments.at(i)->value().c_str() << ")";
                                break;
                            case generated::Pipeline_Node_Argument_Type_pb_string:
                                sserr << "string arguments not allowed as template arguments" << ssthrow;
                            case generated::Pipeline_Node_Argument_Type_pb_complex:
                                this->m_declare_ss << this->m_template_arguments.at(i)->value().c_str();
                                break;
                            default:
                                sserr << "unknown argument type: " << generated::Pipeline_Node_Argument::Type_Name(this->m_constructor_arguments.at(i)->type()) << ssthrow;
                        }
                    }
                    this->m_declare_ss << ">";
                }
                this->m_declare_ss << " node_" << this->nodeIdx();
                if(this->m_constructor_arguments.size() > 0) {
                    this->m_declare_ss << "(";
                    for(int i = 0; i < this->m_constructor_arguments.size(); i++) {
                        if(i > 0)
                            this->m_declare_ss << ",";
                        switch(this->m_constructor_arguments.at(i)->type()) {
                            case generated::Pipeline_Node_Argument_Type_pb_bool:
                                this->m_declare_ss << "bool(" << this->m_constructor_arguments.at(i)->value().c_str() << ")";
                                break;
                            case generated::Pipeline_Node_Argument_Type_pb_int8:
                                this->m_declare_ss << "int8_t(" << this->m_constructor_arguments.at(i)->value().c_str() << ")";
                                break;
                            case generated::Pipeline_Node_Argument_Type_pb_int16:
                                this->m_declare_ss << "int16_t(" << this->m_constructor_arguments.at(i)->value().c_str() << ")";
                                break;
                            case generated::Pipeline_Node_Argument_Type_pb_int32:
                                this->m_declare_ss << "int32_t(" << this->m_constructor_arguments.at(i)->value().c_str() << ")";
                                break;
                            case generated::Pipeline_Node_Argument_Type_pb_int64:
                                this->m_declare_ss << "int64_t(" << this->m_constructor_arguments.at(i)->value().c_str() << ")";
                                break;
                            case generated::Pipeline_Node_Argument_Type_pb_uint8:
                                this->m_declare_ss << "uint8_t(" << this->m_constructor_arguments.at(i)->value().c_str() << ")";
                                break;
                            case generated::Pipeline_Node_Argument_Type_pb_uint16:
                                this->m_declare_ss << "uint16_t(" << this->m_constructor_arguments.at(i)->value().c_str() << ")";
                                break;
                            case generated::Pipeline_Node_Argument_Type_pb_uint32:
                                this->m_declare_ss << "uint32_t(" << this->m_constructor_arguments.at(i)->value().c_str() << ")";
                                break;
                            case generated::Pipeline_Node_Argument_Type_pb_uint64:
                                this->m_declare_ss << "uint64_t(" << this->m_constructor_arguments.at(i)->value().c_str() << ")";
                                break;
                            case generated::Pipeline_Node_Argument_Type_pb_float32:
                                this->m_declare_ss << "float(" << this->m_constructor_arguments.at(i)->value().c_str() << ")";
                                break;
                            case generated::Pipeline_Node_Argument_Type_pb_float64:
                                this->m_declare_ss << "double(" << this->m_constructor_arguments.at(i)->value().c_str() << ")";
                                break;
                            case generated::Pipeline_Node_Argument_Type_pb_string:
                                this->m_declare_ss << "std::string(\"" << this->m_constructor_arguments.at(i)->value().c_str() << "\")";
                                break;
                            case generated::Pipeline_Node_Argument_Type_pb_complex:
                                this->m_declare_ss << this->m_constructor_arguments.at(i)->value().c_str();
                                break;
                            default:
                                sserr << "unknown argument type: " << generated::Pipeline_Node_Argument::Type_Name(this->m_constructor_arguments.at(i)->type()) << ssthrow;
                        }
                    }
                    this->m_declare_ss << ")";
                }
                this->m_declare_ss << ";" << std::endl;

                for(int i = 0; i < this->node().input_size(); i++) {
                    for(int j = 0; j < this->pipeline().node_size(); j++) {
                        if (this->pipeline().node(j).name() == this->node().input(i).node()) {
                            this->m_connect_ss << "\t/* " << std::endl;
                            this->m_connect_ss << "\t * connect output port " << this->node().input(i).port() << " of node \"" << this->pipeline().node(j).name().c_str() << "\"" << std::endl;
                            this->m_connect_ss << "\t * to input port " << i << " of node \"" << this->node().name().c_str() << "\"" << std::endl;
                            this->m_connect_ss << "\t */" << std::endl;
                            this->m_connect_ss << "\ttry {" << std::endl;
                            this->m_connect_ss << "\t\tconnect_port(node_" << j << "," << this->node().input(i).port() << ",node_" << this->nodeIdx() << "," << i << ");" << std::endl;
                            this->m_connect_ss << "\t} catch(std::runtime_error& err) {" << std::endl;
                            this->m_connect_ss << "\t\tsserr << \"error while connecting port " << i << " of node \\\"node_" << this->nodeIdx() << "\\\" (\\\"" << this->node().name().c_str() << "\\\") with port " << this->node().input(i).port() << " of node \\\"node_" << j << "\\\" (\\\"" << this->pipeline().node(j).name().c_str() << "\\\")\\n\" << err.what() << ssthrow;" << std::endl;
                            this->m_connect_ss << "\t}" << std::endl;

                            break;
                        }
                    }
                }
            }
        public:
            CodeGen() :
                    m_pipeline(),
                    m_node_idx(-1),
                    m_header(),
                    m_classname(),
                    m_template_arguments(),
                    m_constructor_arguments(),
                    m_inputs() {}

            class SetPipeline {
            private:
                generated::Pipeline& m_pipeline;
            public:
                SetPipeline(generated::Pipeline &m_pipeline) : m_pipeline(m_pipeline) {

                }

                generated::Pipeline& pipeline() const {
                    return this->m_pipeline;
                }
            };

            class SetNode {
            private:
                int m_node_idx;
            public:
                SetNode(int node_idx) : m_node_idx(node_idx) {

                }

                int nodeIdx() const {
                    return this->m_node_idx;
                }
            };

            class SetHeader {
            private:
                std::string m_header;
            public:
                SetHeader(std::string header) : m_header(header) {

                }

                std::string header() const {
                    return this->m_header;
                }
            };

            class SetClass {
            private:
                std::string m_classname;
            public:
                SetClass(std::string classname) : m_classname(classname) {

                }

                std::string classname() const {
                    return this->m_classname;
                }
            };

            class SetInput {
            private:
                std::string m_name;
            public:
                SetInput(std::string name, uint32_t idx) : m_name(name) {

                }

                std::string name() const {
                    return this->m_name;
                }
            };

            class SetNoInput { };

            class SetTemplateArgument {
            private:
                std::string m_name;
                generated::Pipeline_Node_Argument_Type m_type;
                bool m_optional;
                std::string m_default;
            public:
                SetTemplateArgument(std::string name, generated::Pipeline_Node_Argument_Type type) {
                    this->m_name = name;
                    this->m_type = type;
                    this->m_optional = false;
                    this->m_default = "";
                }

                SetTemplateArgument(std::string name, generated::Pipeline_Node_Argument_Type type, std::string _default) {
                    this->m_name = name;
                    this->m_type = type;
                    this->m_optional = true;
                    this->m_default = _default;
                }

                std::string name() const {
                    return this->m_name;
                }

                generated::Pipeline_Node_Argument_Type type() const {
                    return this->m_type;
                }

                bool optional() const {
                    return this->m_optional;
                }

                std::string _default() const {
                    return this->m_default;
                }

            };

            class SetConstructorArgument {
            private:
                std::string m_name;
                generated::Pipeline_Node_Argument_Type m_type;
                bool m_optional;
                std::string m_default;
            public:
                SetConstructorArgument(std::string name, generated::Pipeline_Node_Argument_Type type) {
                    this->m_name = name;
                    this->m_type = type;
                    this->m_optional = false;
                    this->m_default = "";
                }

                SetConstructorArgument(std::string name, generated::Pipeline_Node_Argument_Type type, std::string _default) {
                    this->m_name = name;
                    this->m_type = type;
                    this->m_optional = true;
                    this->m_default = _default;
                }

                std::string name() const {
                    return this->m_name;
                }

                generated::Pipeline_Node_Argument_Type type() const {
                    return this->m_type;
                }

                bool optional() const {
                    return this->m_optional;
                }

                std::string _default() const {
                    return this->m_default;
                }
            };

            CodeGen& operator<<(const SetPipeline& pipeline) {
                this->m_node_idx = -1;
                this->m_header = "";
                this->m_classname = "";
                this->m_template_arguments.clear();
                this->m_constructor_arguments.clear();
                this->m_inputs.clear();

                this->m_include_ss.clear();
                this->m_declare_ss.clear();
                this->m_connect_ss.clear();

                this->m_pipeline = pipeline.pipeline();
                return *this;
            }

            CodeGen& operator<<(const SetNode& node) {
                sserr << sscond(node.nodeIdx() >= this->pipeline().node_size()) << "node index out of bounds" << ssthrow;

                if(this->m_node_idx >= 0)
                    this->generate();

                this->m_node_idx = -1;
                this->m_header = "";
                this->m_classname = "";
                this->m_template_arguments.clear();
                this->m_constructor_arguments.clear();
                this->m_inputs.clear();

                this->m_node_idx = node.nodeIdx();
                return *this;
            }

            CodeGen& operator<<(const SetHeader& header) {
                this->m_header = header.header();
                return *this;
            }

            CodeGen& operator<<(const SetClass& clazz) {
                this->m_classname = clazz.classname();
                return *this;
            }

            CodeGen& operator<<(const SetTemplateArgument& templateArgument) {
                generated::Pipeline_Node_Argument* argument;

                for(int i = 0; i < this->node().argument_size(); i++) {
                    argument = (generated::Pipeline_Node_Argument*)&(this->node().argument(i));
                    if(argument->name() != templateArgument.name())
                        continue;

                    sserr << ssequal(argument->type(),templateArgument.type())
                          << "invlaid type for argument \"" << templateArgument.name()
                          << "\" expected type \"" << generated::Pipeline_Node_Argument::Type_Name(templateArgument.type())
                          << "\" but got: \"" << generated::Pipeline_Node_Argument::Type_Name(argument->type());

                    this->pushTemplateArgument(argument);
                    return *this;
                }

                if(templateArgument.optional()) {
                    argument = this->node().add_argument();
                    argument->set_name(templateArgument.name());
                    argument->set_type(templateArgument.type());
                    argument->set_value(templateArgument._default());

                    this->pushTemplateArgument(argument);
                    return *this;
                }

                sserr   << "Missing argument \"" << templateArgument.name()
                        << "\" of type \"" << generated::Pipeline_Node_Argument::Type_Name(templateArgument.type())
                        << "\" in node \"" << this->node().name() << "\" (index: " << this->nodeIdx() << ")" << ssthrow;
            }

            CodeGen& operator<<(const SetConstructorArgument& constructorArgument) {
                generated::Pipeline_Node_Argument* argument;

                for(int i = 0; i < this->node().argument_size(); i++) {
                    argument = (generated::Pipeline_Node_Argument*)&(this->node().argument(i));
                    if(argument->name() != constructorArgument.name())
                        continue;

                    sserr << ssequal(argument->type(),constructorArgument.type())
                          << "invlaid type for argument \"" << constructorArgument.name()
                          << "\" expected type \"" << generated::Pipeline_Node_Argument::Type_Name(constructorArgument.type())
                          << "\" but got: \"" << generated::Pipeline_Node_Argument::Type_Name(argument->type());

                    this->pushConstructorArgument(argument);
                    return *this;
                }

                if(constructorArgument.optional()) {
                    argument = this->node().add_argument();
                    argument->set_name(constructorArgument.name());
                    argument->set_type(constructorArgument.type());
                    argument->set_value(constructorArgument._default());

                    this->pushConstructorArgument(argument);
                    return *this;
                }

                sserr   << "Missing argument \"" << constructorArgument.name()
                        << "\" of type \"" << generated::Pipeline_Node_Argument::Type_Name(constructorArgument.type())
                        << "\" in node \"" << this->node().name() << "\" (index: " << this->nodeIdx() << ", debug: " << this->node().Utf8DebugString() << ")" << ssthrow;
            }

            CodeGen& operator<<(const SetInput& input) {
                int i;

                sserr << sscond(this->m_inputs.size() >= this->node().input_size()) << "input \"" << input.name() << "\" is missing in node \"" << this->node().name() << "\"" << ssthrow;

                for(i = 0; i < this->pipeline().node_size(); i++)
                    if(this->pipeline().node(i).name() == this->node().input(this->m_inputs.size()).node())
                        break;

                sserr << sscond(i >= this->pipeline().node_size()) << "cannot find node \"" << this->node().input(this->m_inputs.size()).node() << "\" as input \"" << input.name() << "\" for node \"" << this->node().name() << "\"" << ssthrow;

                this->pushInput(&(this->node().input(this->m_inputs.size())));
            }

            CodeGen& operator<<(const SetNoInput& input) {
                sserr << sscond(this->node().input_size()>0) << "node \"" << this->node().name() << "\" has no input port" << ssthrow;
            }

            std::string cpp() {
                std::string include_s;
                std::string declare_s;
                std::string connect_s;
                std::stringstream ss;

                include_s = this->m_include_ss.str();
                declare_s = this->m_declare_ss.str();
                connect_s = this->m_connect_ss.str();

                ss << "/*  " << std::endl;
                ss << " *  " << std::endl;
                ss << " */ " << std::endl;
                ss << "#include <core/Pipeline.h>" << std::endl;
                ss << include_s << std::endl;
                ss << declare_s << std::endl;
                ss << "extern \"C\" void create_pipeline(saliency_sandbox::core::Pipeline& pipeline) {" << std::endl;
                ss << connect_s << std::endl;
                for(int i = 0; i < this->pipeline().node_size(); i++)
                    ss << "\tpipeline.pushNode(\"" << this->pipeline().node(i).name().c_str() << "\",&node_" << i << ");" << std::endl;
                ss << "}" << std::endl;
                return ss.str();
            }

            static std::string cpp(boost::filesystem::path config) {
                int fd;
                boost::scoped_ptr<google::protobuf::io::FileInputStream> is;
                generated::Pipeline pipeline;

                fd = open(config.c_str(),O_RDONLY);
                sserr << sscond(fd < 0) << "cannot open file: " << config << ssthrow;

                is.reset(new google::protobuf::io::FileInputStream(fd));
                is->SetCloseOnDelete(true);
                sserr << sscond(!google::protobuf::TextFormat::Parse(is.get(),&pipeline)) << "error while parsing configuration: " << config << ssthrow;

                return CodeGen::cpp(pipeline);
            }

            static std::string cpp(generated::Pipeline& pipeline) {
                CodeGen gen;

                gen << SetPipeline(pipeline);
                for(int i = 0; i < pipeline.node_size(); i++) {
                    gen << SetNode(i);
                    switch(pipeline.node(i).type()) {
                        case generated::Pipeline_Node_Type_KittiReader:
                            gen << SetHeader("kitti/KittiReader");
                            gen << SetClass("saliency_sandbox::kitti::KittiReader");
                            gen << SetConstructorArgument("path",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_string);
                            gen << SetNoInput();
                            break;
                        case generated::Pipeline_Node_Type_KittiCalibrationReader:
                            gen << SetHeader("kitti/KittiReader");
                            gen << SetClass("saliency_sandbox::kitti::CalibrationReader");
                            gen << SetConstructorArgument("path",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_string);
                            gen << SetNoInput();
                            break;
                        case generated::Pipeline_Node_Type_KittiLeftGrayImageReader:
                            gen << SetHeader("kitti/ImageReader");
                            gen << SetClass("saliency_sandbox::kitti::LeftGrayImageReader");
                            gen << SetConstructorArgument("path",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_string);
                            gen << SetNoInput();
                            break;
                        case generated::Pipeline_Node_Type_KittiRightGrayImageReader:
                            gen << SetHeader("kitti/ImageReader");
                            gen << SetClass("saliency_sandbox::kitti::RightGrayImageReader");
                            gen << SetConstructorArgument("path",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_string);
                            gen << SetNoInput();
                            break;
                        case generated::Pipeline_Node_Type_KittiLeftRGBImageReader:
                            gen << SetHeader("kitti/ImageReader");
                            gen << SetClass("saliency_sandbox::kitti::LeftRGBImageReader");
                            gen << SetConstructorArgument("path",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_string);
                            gen << SetNoInput();
                            break;
                        case generated::Pipeline_Node_Type_KittiRightRGBImageReader:
                            gen << SetHeader("kitti/ImageReader");
                            gen << SetClass("saliency_sandbox::kitti::RightRGBImageReader");
                            gen << SetConstructorArgument("path",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_string);
                            gen << SetNoInput();
                            break;
                        case generated::Pipeline_Node_Type_KittiOXTSReader:
                            gen << SetHeader("kitti/OXTSReader");
                            gen << SetClass("saliency_sandbox::kitti::OXTSReader");
                            gen << SetConstructorArgument("path",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_string);
                            gen << SetNoInput();
                            break;
                        case generated::Pipeline_Node_Type_KittiTrackletReader:
                            gen << SetHeader("kitti/TrackletReader");
                            gen << SetClass("saliency_sandbox::kitti::TrackletReader");
                            gen << SetConstructorArgument("path",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_string);
                            gen << SetNoInput();
                            break;
                        case generated::Pipeline_Node_Type_KittiVelodyneReader:
                            gen << SetHeader("kitti/VelodyneReader");
                            gen << SetClass("saliency_sandbox::kitti::VelodyneReader");
                            gen << SetConstructorArgument("path",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_string);
                            gen << SetNoInput();
                            break;
                        case generated::Pipeline_Node_Type_IOCSVReader:
                            gen << SetHeader("io/CSVReader");
                            gen << SetClass("saliency_sandbox::io::CSVReader");
                            gen << SetTemplateArgument("seperator",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_string);
                            for(int j = 0; j < pipeline.node(i).argument_size()-2;j++)
                                gen << SetTemplateArgument("field_" + j,generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_complex);
                            gen << SetConstructorArgument("path",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_string);
                            gen << SetNoInput();
                            break;
                        case generated::Pipeline_Node_Type_IODirectoryScanner:
                            gen << SetHeader("io/DirectoryScanner");
                            gen << SetClass("saliency_sandbox::io::DirectoryScanner");
                            gen << SetConstructorArgument("path",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_string);
                            gen << SetNoInput();
                            break;
                        case generated::Pipeline_Node_Type_IOImageReader:
                            gen << SetHeader("io/ImageReader");
                            gen << SetClass("saliency_sandbox::io::_ImageReader");
                            gen << SetTemplateArgument("width",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_uint32,RES_WIDTH_S(RESOLUTION) );
                            gen << SetTemplateArgument("height",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_uint32,RES_HEIGHT_S(RESOLUTION));
                            gen << SetTemplateArgument("image_format",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_complex,"saliency_sandbox::utils::_RGBImage");
                            gen << SetConstructorArgument("path",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_string);
                            gen << SetNoInput();
                            break;
                        case generated::Pipeline_Node_Type_IOImageShow:
                            gen << SetHeader("io/ImageShow");
                            gen << SetClass("saliency_sandbox::io::ImageShow");
                            gen << SetConstructorArgument("window",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_string);
                            gen << SetInput("image",0);
                            break;
                        case generated::Pipeline_Node_Type_IOImageWriter:
                            gen << SetHeader("io/ImageWrite");
                            gen << SetClass("saliency_sandbox::io::ImageWrite");
                            gen << SetConstructorArgument("path",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_string);
                            gen << SetInput("image",0);
                            break;
                        case generated::Pipeline_Node_Type_IOVideoReader:
                            gen << SetHeader("io/VideoReader");
                            gen << SetClass("saliency_sandbox::io::VideoReader");
                            gen << SetTemplateArgument("width",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_uint32,RES_WIDTH_S(RESOLUTION) );
                            gen << SetTemplateArgument("height",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_uint32,RES_HEIGHT_S(RESOLUTION));
                            gen << SetConstructorArgument("path",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_string);
                            gen << SetNoInput();
                            break;
                        case generated::Pipeline_Node_Type_IOVideoWriter:
                            gen << SetHeader("io/VideoWriter");
                            gen << SetClass("saliency_sandbox::io::VideoWriter");
                            gen << SetConstructorArgument("path",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_string);
                            gen << SetInput("image",0);
                            break;
                        case generated::Pipeline_Node_Type_UtilsImageConverter:
                            gen << SetHeader("utils/Image");
                            gen << SetClass("saliency_sandbox::utils::_ImageConvert");
                            gen << SetTemplateArgument("width",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_uint32,RES_WIDTH_S(RESOLUTION) );
                            gen << SetTemplateArgument("height",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_uint32,RES_HEIGHT_S(RESOLUTION));
                            gen << SetTemplateArgument("input",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_complex,"saliency_sandbox::utils::_RGBImage");
                            gen << SetTemplateArgument("output",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_complex);
                            gen << SetInput("image",0);
                            break;
                        case generated::Pipeline_Node_Type_SaliencySpectralWhitening:
                            gen << SetHeader("saliency/activation/Spectral");
                            gen << SetClass("saliency_sandbox::saliency::activation::_Spectral");
                            gen << SetTemplateArgument("width",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_uint32,RES_WIDTH_S(RESOLUTION) );
                            gen << SetTemplateArgument("height",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_uint32,RES_HEIGHT_S(RESOLUTION));
                            gen << SetInput("feature",0);
                            break;
                        case generated::Pipeline_Node_Type_SaliencyBooleanMaps:
                            gen << SetHeader("saliency/activation/BooleanMaps");
                            gen << SetClass("saliency_sandbox::saliency::activation::_BooleanMaps");
                            gen << SetTemplateArgument("width",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_uint32,RES_WIDTH_S(RESOLUTION) );
                            gen << SetTemplateArgument("height",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_uint32,RES_HEIGHT_S(RESOLUTION));
                            gen << SetInput("feature",0);
                            break;
                        case generated::Pipeline_Node_Type_SaliencyHighpass:
                            gen << SetHeader("saliency/activation/HighPass");
                            gen << SetClass("saliency_sandbox::saliency::activation::_HighPass");
                            gen << SetTemplateArgument("width",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_uint32,RES_WIDTH_S(RESOLUTION) );
                            gen << SetTemplateArgument("height",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_uint32,RES_HEIGHT_S(RESOLUTION));
                            gen << SetInput("feature",0);
                            break;
                        case generated::Pipeline_Node_Type_SaliencyPearson:
                            gen << SetHeader("saliency/evaluation/Pearson");
                            gen << SetClass("saliency_sandbox::saliency::evaluation::_Pearson");
                            gen << SetTemplateArgument("width",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_uint32,RES_WIDTH_S(RESOLUTION) );
                            gen << SetTemplateArgument("height",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_uint32,RES_HEIGHT_S(RESOLUTION));
                            gen << SetInput("saliency",0);
                            gen << SetInput("fixation",0);
                            break;
                        case generated::Pipeline_Node_Type_UtilsFPSCounter:
                            gen << SetHeader("utils/FPSCounter");
                            gen << SetClass("saliency_sandbox::utils::FPSCounter");
                            gen << SetInput("process",0);
                            break;
                        case generated::Pipeline_Node_Type_UtilsMatrixSplit:
                            gen << SetHeader("utils/Matrix");
                            gen << SetClass("saliency_sandbox::utils::_MatrixSplitt");
                            gen << SetTemplateArgument("width",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_uint32,RES_WIDTH_S(RESOLUTION) );
                            gen << SetTemplateArgument("height",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_uint32,RES_HEIGHT_S(RESOLUTION));
                            gen << SetTemplateArgument("type",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_complex);
                            gen << SetInput("matrix",0);
                            break;
                        case generated::Pipeline_Node_Type_UtilsMatrixMerge:
                            gen << SetHeader("utils/Matrix");
                            gen << SetClass("saliency_sandbox::utils::_MatrixMerge");
                            gen << SetTemplateArgument("width",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_uint32,RES_WIDTH_S(RESOLUTION) );
                            gen << SetTemplateArgument("height",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_uint32,RES_HEIGHT_S(RESOLUTION));
                            gen << SetTemplateArgument("type",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_complex);
                            break;
                        case generated::Pipeline_Node_Type_UtilsMatrixSum:
                            gen << SetHeader("utils/Matrix");
                            gen << SetClass("saliency_sandbox::utils::_MatrixSum");
                            gen << SetTemplateArgument("width",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_uint32,RES_WIDTH_S(RESOLUTION) );
                            gen << SetTemplateArgument("height",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_uint32,RES_HEIGHT_S(RESOLUTION));
                            gen << SetTemplateArgument("type",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_complex);
                            gen << SetInput("matrix",0);
                            break;
                        case generated::Pipeline_Node_Type_Plot:
                            gen << SetHeader("plot/Plot");
                            gen << SetClass("saliency_sandbox::plot::Plot");
                            gen << SetInput("line 0",0);
                            gen << SetConstructorArgument("line0",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_string,"line0");
                            sserr << sscond(pipeline.node(i).input_size() >= 10) << "the maximum number of input of node \"" << pipeline.node(i).name()
                                  << "\" is 10. found " << pipeline.node(i).input_size() << ssthrow;
                            for(int j = 1; j < pipeline.node(i).input_size() && j < 10; j++) {
                                std::stringstream ss;

                                ss << "line" << j;
                                gen << SetInput(ss.str(), i);
                                gen << SetConstructorArgument(ss.str(),generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_string,ss.str());
                            }
                            break;
                        default:
                            sserr << "unknown node type: " << generated::Pipeline_Node::Type_Name(pipeline.node(i).type()) << ssthrow;
                    }
                }
                gen.generate();
                return gen.cpp();
            }
        };
    }
}

#endif //CODE_GEN_H
