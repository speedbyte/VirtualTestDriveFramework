//
// Created by geislerd on 15.03.17.
//

#include <core/CodeGen.h>
#include <utils/Error.h>
#include <config.h>
#include <boost/scoped_ptr.hpp>
#include <fcntl.h>

namespace saliency_sandbox {

    namespace core {

            const generated::Pipeline& CodeGen::pipeline() {
                return this->m_pipeline;
            }

            const int CodeGen::nodeIdx() {
                return this->m_node_idx;
            }

            generated::Pipeline_Node& CodeGen::node() {
                sserr << sscond(this->nodeIdx() < 0 || this->nodeIdx() >= this->pipeline().node_size()) << "node not found with index: " << this->nodeIdx() << ssthrow;
                return (generated::Pipeline_Node&)this->pipeline().node(this->nodeIdx());
            }

            std::string CodeGen::header() {
                return this->m_header;
            }

            std::string CodeGen::classname() {
                return this->m_classname;
            }

            void CodeGen::pushTemplateArgument(const generated::Pipeline_Node_Argument *argument) {
                this->m_template_arguments.push_back(argument);
            }

            void CodeGen::pushConstructorArgument(const generated::Pipeline_Node_Argument *argument) {
                this->m_constructor_arguments.push_back(argument);
            }

            void CodeGen::pushInput(const generated::Pipeline_Node_Input *input) {
                this->m_inputs.push_back(input);
            }

            void CodeGen::generate() {

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

                this->m_init_ss << "\t/*" << std::endl;
                this->m_init_ss << "\t * set properties of node \"" << this->node().name() << "\"" << std::endl;
                this->m_init_ss << "\t */" << std::endl;
                for(int i = 0; i < this->node().property_size(); i++) {
                    switch(this->node().property(i).type()) {
                        case generated::Pipeline_Node_Property_Type_pb_bool:
                            this->m_init_ss << "\tnode_" << this->m_node_idx << ".properties()->template set<bool>(\"" << this->node().property(i).name() << "\",bool(" << this->node().property(i).value() << "));";
                            break;
                        case generated::Pipeline_Node_Property_Type_pb_int:
                            this->m_init_ss << "\tnode_" << this->m_node_idx << ".properties()->template set<int>(\"" << this->node().property(i).name() << "\",int(" << this->node().property(i).value() << "));";
                            break;
                        case generated::Pipeline_Node_Property_Type_pb_float:
                            this->m_init_ss << "\tnode_" << this->m_node_idx << ".properties()->template set<float>(\"" << this->node().property(i).name() <<"\",float(" << this->node().property(i).value() << "));";
                            break;
                        case generated::Pipeline_Node_Property_Type_pb_string:
                            this->m_init_ss << "\tnode_" << this->m_node_idx << ".properties()->template set<std::string>(\"" << this->node().property(i).name() << "\",std::string(\"" << this->node().property(i).value() << "\"));";
                            break;
                        default:
                            sserr << "unknown property type: " << generated::Pipeline_Node_Property::Type_Name(this->node().property(i).type()) << ssthrow;
                    }
                }
                this->m_init_ss << std::endl;

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

        CodeGen::CodeGen() :
                    m_pipeline(),
                    m_node_idx(-1),
                    m_header(),
                    m_classname(),
                    m_template_arguments(),
                    m_constructor_arguments(),
                    m_inputs() {}


                CodeGen::SetPipeline::SetPipeline(generated::Pipeline &m_pipeline) : m_pipeline(m_pipeline) {

                }

                generated::Pipeline& CodeGen::SetPipeline::pipeline() const {
                    return this->m_pipeline;
                }

                CodeGen::SetNode::SetNode(int node_idx) : m_node_idx(node_idx) {

                }

                int CodeGen::SetNode::nodeIdx() const {
                    return this->m_node_idx;
                }

                CodeGen::SetHeader::SetHeader(std::string header) : m_header(header) {

                }

                std::string CodeGen::SetHeader::header() const {
                    return this->m_header;
                }

                CodeGen::SetClass::SetClass(std::string classname) : m_classname(classname) {

                }

                std::string CodeGen::SetClass::classname() const {
                    return this->m_classname;
                }

                CodeGen::SetInput::SetInput(std::string name, uint32_t idx) : m_name(name) {

                }

                std::string CodeGen::SetInput::name() const {
                    return this->m_name;
                }

                CodeGen::SetTemplateArgument::SetTemplateArgument(std::string name, generated::Pipeline_Node_Argument_Type type) {
                    this->m_name = name;
                    this->m_type = type;
                    this->m_optional = false;
                    this->m_default = "";
                }

        CodeGen::SetTemplateArgument::SetTemplateArgument(std::string name, generated::Pipeline_Node_Argument_Type type, std::string _default) {
                    this->m_name = name;
                    this->m_type = type;
                    this->m_optional = true;
                    this->m_default = _default;
                }

                std::string CodeGen::SetTemplateArgument::name() const {
                    return this->m_name;
                }

                generated::Pipeline_Node_Argument_Type CodeGen::SetTemplateArgument::type() const {
                    return this->m_type;
                }

                bool CodeGen::SetTemplateArgument::optional() const {
                    return this->m_optional;
                }

                std::string CodeGen::SetTemplateArgument::_default() const {
                    return this->m_default;
                }

                CodeGen::SetConstructorArgument::SetConstructorArgument(std::string name, generated::Pipeline_Node_Argument_Type type) {
                    this->m_name = name;
                    this->m_type = type;
                    this->m_optional = false;
                    this->m_default = "";
                }

        CodeGen::SetConstructorArgument::SetConstructorArgument(std::string name, generated::Pipeline_Node_Argument_Type type, std::string _default) {
                    this->m_name = name;
                    this->m_type = type;
                    this->m_optional = true;
                    this->m_default = _default;
                }

                std::string CodeGen::SetConstructorArgument::name() const {
                    return this->m_name;
                }

                generated::Pipeline_Node_Argument_Type CodeGen::SetConstructorArgument::type() const {
                    return this->m_type;
                }

                bool CodeGen::SetConstructorArgument::optional() const {
                    return this->m_optional;
                }

                std::string CodeGen::SetConstructorArgument::_default() const {
                    return this->m_default;
                }

                CodeGen::SetProperty::SetProperty(std::string name, generated::Pipeline_Node_Property_Type type) {
                    this->m_name = name;
                    this->m_type = type;
                }

                std::string CodeGen::SetProperty::name() const {
                    return this->m_name;
                }

                generated::Pipeline_Node_Property_Type CodeGen::SetProperty::type() const {
                    return this->m_type;
                }

            CodeGen& CodeGen::operator<<(const CodeGen::SetPipeline& pipeline) {
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

            CodeGen& CodeGen::operator<<(const CodeGen::SetNode& node) {
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

            CodeGen& CodeGen::operator<<(const CodeGen::SetHeader& header) {
                this->m_header = header.header();
                return *this;
            }

            CodeGen& CodeGen::operator<<(const CodeGen::SetClass& clazz) {
                this->m_classname = clazz.classname();
                return *this;
            }

            CodeGen& CodeGen::operator<<(const CodeGen::SetTemplateArgument& templateArgument) {
                generated::Pipeline_Node_Argument* argument;

                for(int i = 0; i < this->node().argument_size(); i++) {
                    argument = (generated::Pipeline_Node_Argument*)&(this->node().argument(i));
                    if(argument->name() != templateArgument.name())
                        continue;

                    sserr << ssequal(argument->type(),templateArgument.type())
                          << "invlaid type for argument \"" << templateArgument.name()
                          << "\" expected type \"" << generated::Pipeline_Node_Argument::Type_Name(templateArgument.type())
                          << "\" but got: \"" << generated::Pipeline_Node_Argument::Type_Name(argument->type()) << ssthrow;

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

            CodeGen& CodeGen::operator<<(const CodeGen::SetConstructorArgument& constructorArgument) {
                generated::Pipeline_Node_Argument* argument;

                for(int i = 0; i < this->node().argument_size(); i++) {
                    argument = (generated::Pipeline_Node_Argument*)&(this->node().argument(i));
                    if(argument->name() != constructorArgument.name())
                        continue;

                    sserr << ssequal(argument->type(),constructorArgument.type())
                          << "invlaid type for argument \"" << constructorArgument.name()
                          << "\" expected type \"" << generated::Pipeline_Node_Argument::Type_Name(constructorArgument.type())
                          << "\" but got: \"" << generated::Pipeline_Node_Argument::Type_Name(argument->type()) << ssthrow;

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

            CodeGen& CodeGen::operator<<(const CodeGen::SetInput& input) {
                int i;

                sserr << sscond(this->m_inputs.size() >= this->node().input_size()) << "input \"" << input.name() << "\" is missing in node \"" << this->node().name() << "\"" << ssthrow;

                for(i = 0; i < this->pipeline().node_size(); i++)
                    if(this->pipeline().node(i).name() == this->node().input(this->m_inputs.size()).node())
                        break;

                sserr << sscond(i >= this->pipeline().node_size()) << "cannot find node \"" << this->node().input(this->m_inputs.size()).node() << "\" as input \"" << input.name() << "\" for node \"" << this->node().name() << "\"" << ssthrow;

                this->pushInput(&(this->node().input(this->m_inputs.size())));
            }

            CodeGen& CodeGen::operator<<(const SetNoInput& input) {
                sserr << sscond(this->node().input_size()>0) << "node \"" << this->node().name() << "\" has no input port" << ssthrow;
            }

            CodeGen& CodeGen::operator<<(const CodeGen::SetProperty& sproperty) {
                generated::Pipeline_Node_Property* property;

                for(int i = 0; i < this->node().property_size(); i++) {
                    property = (generated::Pipeline_Node_Property*)&(this->node().property(i));
                    if(property->name() != sproperty.name())
                        continue;

                    sserr << ssequal(property->type(),sproperty.type())
                          << "invlaid type for property \"" << sproperty.name()
                          << "\" expected type \"" << generated::Pipeline_Node_Property::Type_Name(sproperty.type())
                          << "\" but got: \"" << generated::Pipeline_Node_Property::Type_Name(property->type()) << ssthrow;
                }
            }

            std::string CodeGen::cpp() {
                std::string include_s;
                std::string declare_s;
                std::string init_s;
                std::string connect_s;
                std::stringstream ss;

                include_s = this->m_include_ss.str();
                declare_s = this->m_declare_ss.str();
                init_s = this->m_init_ss.str();
                connect_s = this->m_connect_ss.str();

                ss << "/*  " << std::endl;
                ss << " *  " << std::endl;
                ss << " */ " << std::endl;
                ss << "#include <core/Pipeline.h>" << std::endl;
                ss << include_s << std::endl;
                ss << declare_s << std::endl;
                ss << "extern \"C\" void create_pipeline(saliency_sandbox::core::Pipeline& pipeline) {" << std::endl;
                ss << init_s << std::endl;
                ss << connect_s << std::endl;
                for(int i = 0; i < this->pipeline().node_size(); i++)
                    ss << "\tpipeline.pushNode(\"" << this->pipeline().node(i).name().c_str() << "\",&node_" << i << ");" << std::endl;
                ss << "}" << std::endl;
                ss << "extern \"C\" saliency_sandbox::core::INode* new_pipeline() {" << std::endl;
                ss << "\tsaliency_sandbox::core::Pipeline* pipeline = new saliency_sandbox::core::Pipeline();" << std::endl;
                ss << "\tcreate_pipeline(*pipeline);" << std::endl;
                ss << "\tpipeline->initialize();" << std::endl;
                ss << "\treturn(pipeline);" << std::endl;
                ss << "}" << std::endl;
                return ss.str();
            }

            std::string CodeGen::cpp(boost::filesystem::path config) {
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

            std::string CodeGen::cpp(generated::Pipeline& pipeline) {
                CodeGen gen;

                sserr << sscond(!pipeline.node_size()) << "Pipeline \"" << pipeline.name() << "\" is empty" << ssthrow;

                gen << SetPipeline(pipeline);
                for(int i = 0; i < pipeline.node_size(); i++) {
                    gen << SetNode(i);
                    switch(pipeline.node(i).type()) {
                        case generated::Pipeline_Node_Type_KittiReader:
                            gen << CodeGen::SetHeader("kitti/KittiReader");
                            gen << CodeGen::SetClass("saliency_sandbox::kitti::KittiReader");
                            gen << CodeGen::SetConstructorArgument("path",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_string);
                            gen << CodeGen::SetNoInput();
                            break;
                        case generated::Pipeline_Node_Type_KittiCalibrationReader:
                            gen << CodeGen::SetHeader("kitti/KittiReader");
                            gen << CodeGen::SetClass("saliency_sandbox::kitti::CalibrationReader");
                            gen << CodeGen::SetConstructorArgument("path",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_string);
                            gen << CodeGen::SetNoInput();
                            break;
                        case generated::Pipeline_Node_Type_KittiLeftGrayImageReader:
                            gen << CodeGen::SetHeader("kitti/ImageReader");
                            gen << CodeGen::SetClass("saliency_sandbox::kitti::LeftGrayImageReader");
                            gen << CodeGen::SetConstructorArgument("path",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_string);
                            gen << CodeGen::SetNoInput();
                            break;
                        case generated::Pipeline_Node_Type_KittiRightGrayImageReader:
                            gen << CodeGen::SetHeader("kitti/ImageReader");
                            gen << CodeGen::SetClass("saliency_sandbox::kitti::RightGrayImageReader");
                            gen << CodeGen::SetConstructorArgument("path",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_string);
                            gen << CodeGen::SetNoInput();
                            break;
                        case generated::Pipeline_Node_Type_KittiLeftRGBImageReader:
                            gen << CodeGen::SetHeader("kitti/ImageReader");
                            gen << CodeGen::SetClass("saliency_sandbox::kitti::LeftRGBImageReader");
                            gen << CodeGen::SetConstructorArgument("path",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_string);
                            gen << CodeGen::SetNoInput();
                            break;
                        case generated::Pipeline_Node_Type_KittiRightRGBImageReader:
                            gen << CodeGen::SetHeader("kitti/ImageReader");
                            gen << CodeGen::SetClass("saliency_sandbox::kitti::RightRGBImageReader");
                            gen << CodeGen::SetConstructorArgument("path",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_string);
                            gen << CodeGen::SetNoInput();
                            break;
                        case generated::Pipeline_Node_Type_KittiOXTSReader:
                            gen << CodeGen::SetHeader("kitti/OXTSReader");
                            gen << CodeGen::SetClass("saliency_sandbox::kitti::OXTSReader");
                            gen << CodeGen::SetConstructorArgument("path",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_string);
                            gen << CodeGen::SetNoInput();
                            break;
                        case generated::Pipeline_Node_Type_KittiOXTSPose:
                            gen << CodeGen::SetHeader("kitti/OXTSPose");
                            gen << CodeGen::SetClass("saliency_sandbox::kitti::OXTSPose");
                            gen << CodeGen::SetInput("oxts",0);
                            gen << CodeGen::SetInput("calib",1);
                            break;
                        case generated::Pipeline_Node_Type_KittiOXTSSelector:
                            gen << CodeGen::SetHeader("kitti/OXTSReader");
                            gen << CodeGen::SetTemplateArgument("channel",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_uint32);
                            gen << CodeGen::SetClass("saliency_sandbox::kitti::OXTSRow::Selector");
                            gen << CodeGen::SetInput("velodyne",0);
                            break;
                        case generated::Pipeline_Node_Type_KittiTrackletReader:
                            gen << CodeGen::SetHeader("kitti/TrackletReader");
                            gen << CodeGen::SetClass("saliency_sandbox::kitti::TrackletReader");
                            gen << CodeGen::SetConstructorArgument("path",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_string);
                            gen << CodeGen::SetNoInput();
                            break;
                        case generated::Pipeline_Node_Type_KittiTrackletSaliency:
                            gen << CodeGen::SetHeader("kitti/TrackletSaliency");
                            gen << CodeGen::SetClass("saliency_sandbox::kitti::TrackletSaliency");
                            gen << CodeGen::SetTemplateArgument("camera",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_complex);
                            gen << CodeGen::SetInput("tracklet",0);
                            gen << CodeGen::SetInput("saliency",1);
                            gen << CodeGen::SetInput("calibration",2);
                            break;
                        case generated::Pipeline_Node_Type_KittiTrackletDraw:
                            gen << CodeGen::SetHeader("kitti/DrawTracklet");
                            gen << CodeGen::SetClass("saliency_sandbox::kitti::DrawTracklet");
                            gen << CodeGen::SetTemplateArgument("camera",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_complex);
                            gen << CodeGen::SetInput("image",0);
                            gen << CodeGen::SetInput("tracklet",1);
                            gen << CodeGen::SetInput("calibration",2);
                            break;
                        case generated::Pipeline_Node_Type_KittiVelodyneReader:
                            gen << CodeGen::SetHeader("kitti/VelodyneReader");
                            gen << CodeGen::SetClass("saliency_sandbox::kitti::VelodyneReader");
                            gen << CodeGen::SetConstructorArgument("path",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_string);
                            gen << CodeGen::SetNoInput();
                            break;
                        case generated::Pipeline_Node_Type_KittiVelodynePolar:
                            gen << CodeGen::SetHeader("kitti/Velodyne2PolarImage");
                            gen << CodeGen::SetClass("saliency_sandbox::kitti::Velodyne2PolarImage");
                            gen << CodeGen::SetInput("velodyne",0);
                            break;
                        case generated::Pipeline_Node_Type_KittiEgoMotion:
                            gen << CodeGen::SetHeader("kitti/EgoMotion");
                            gen << CodeGen::SetClass("saliency_sandbox::kitti::EgoMotion");
                            gen << CodeGen::SetTemplateArgument("camera",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_complex,"saliency_sandbox::kitti::Camera::LEFT_RGB");
                            gen << CodeGen::SetInput("velodyne",0);
                            gen << CodeGen::SetInput("oxts",1);
                            gen << CodeGen::SetInput("calibration",2);
                            break;
                        case generated::Pipeline_Node_Type_KittiDrawTrajectory:
                            gen << CodeGen::SetHeader("kitti/DrawTrajectory");
                            gen << CodeGen::SetClass("saliency_sandbox::kitti::DrawTrajectory");
                            gen << CodeGen::SetTemplateArgument("camera",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_complex,"saliency_sandbox::kitti::Camera::LEFT_RGB");
                            gen << CodeGen::SetTemplateArgument("frames",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_uint32,"20");
                            gen << CodeGen::SetInput("oxts",0);
                            gen << CodeGen::SetInput("calibration",1);
                            gen << CodeGen::SetInput("rgb",2);
                            break;
                        case generated::Pipeline_Node_Type_KittiFOVToCam:
                            gen << CodeGen::SetHeader("kitti/FOVToCam");
                            gen << CodeGen::SetClass("saliency_sandbox::kitti::FOVToCam");
                            gen << CodeGen::SetTemplateArgument("camera",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_complex,"saliency_sandbox::kitti::Camera::LEFT_RGB");
                            gen << CodeGen::SetInput("fov",0);
                            gen << CodeGen::SetInput("depth",1);
                            gen << CodeGen::SetInput("calibration",2);
                            break;
                        case generated::Pipeline_Node_Type_KittiDrawBirdView:
                            gen << CodeGen::SetHeader("kitti/DrawBirdView");
                            gen << CodeGen::SetClass("saliency_sandbox::kitti::DrawBirdView");
                            gen << CodeGen::SetTemplateArgument("frames",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_uint32,"20");
                            gen << CodeGen::SetInput("trajectory",0);
                            gen << CodeGen::SetInput("valodyne",1);
                            gen << CodeGen::SetInput("tracklets",2);
                            gen << CodeGen::SetInput("fov",3);
                            gen << CodeGen::SetInput("depth",4);
                            break;
                        case generated::Pipeline_Node_Type_IOCSVReader:
                            gen << CodeGen::SetHeader("io/CSVReader");
                            gen << CodeGen::SetClass("saliency_sandbox::io::CSVReader");
                            gen << CodeGen::SetTemplateArgument("seperator",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_string);
                            for(int j = 0; j < pipeline.node(i).argument_size()-2;j++)
                                gen << CodeGen::SetTemplateArgument("field_" + j,generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_complex);
                            gen << CodeGen::SetConstructorArgument("path",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_string);
                            gen << CodeGen::SetNoInput();
                            break;
                        case generated::Pipeline_Node_Type_IODirectoryScanner:
                            gen << CodeGen::SetHeader("io/DirectoryScanner");
                            gen << CodeGen::SetClass("saliency_sandbox::io::DirectoryScanner");
                            gen << CodeGen::SetConstructorArgument("path",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_string);
                            gen << CodeGen::SetNoInput();
                            break;
                        case generated::Pipeline_Node_Type_IOImageReader:
                            gen << CodeGen::SetHeader("io/ImageReader");
                            gen << CodeGen::SetClass("saliency_sandbox::io::_ImageReader");
                            gen << CodeGen::SetTemplateArgument("width",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_uint32,RES_WIDTH_S(RESOLUTION) );
                            gen << CodeGen::SetTemplateArgument("height",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_uint32,RES_HEIGHT_S(RESOLUTION));
                            gen << CodeGen::SetTemplateArgument("image_format",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_complex,"saliency_sandbox::utils::_RGBImage");
                            gen << CodeGen::SetConstructorArgument("path",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_string);
                            gen << CodeGen::SetNoInput();
                            break;
                        case generated::Pipeline_Node_Type_IOImageMemoryReader:
                            gen << CodeGen::SetHeader("io/ImageMemoryReader");
                            gen << CodeGen::SetClass("saliency_sandbox::io::_ImageMemoryReader");
                            gen << CodeGen::SetTemplateArgument("width",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_uint32,RES_WIDTH_S(RESOLUTION) );
                            gen << CodeGen::SetTemplateArgument("height",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_uint32,RES_HEIGHT_S(RESOLUTION));
                            gen << CodeGen::SetTemplateArgument("image_format",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_complex,"saliency_sandbox::utils::_RGBImage");
                            //gen << CodeGen::SetInput("image",0);
                            break;
                        case generated::Pipeline_Node_Type_IOImageMemoryWriter:
                            gen << CodeGen::SetHeader("io/ImageMemoryWriter");
                            gen << CodeGen::SetClass("saliency_sandbox::io::ImageMemoryWriter");
                            gen << CodeGen::SetInput("image",0);
                            break;
                        case generated::Pipeline_Node_Type_IOImageShow:
                            gen << CodeGen::SetHeader("io/ImageShow");
                            gen << CodeGen::SetClass("saliency_sandbox::io::ImageShow");
                            gen << CodeGen::SetConstructorArgument("window",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_string);
                            gen << CodeGen::SetInput("image",0);
                            break;
                        case generated::Pipeline_Node_Type_IOImageWriter:
                            gen << CodeGen::SetHeader("io/ImageWriter");
                            gen << CodeGen::SetClass("saliency_sandbox::io::ImageWriter");
                            gen << CodeGen::SetConstructorArgument("path",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_string);
                            gen << CodeGen::SetInput("image",0);
                            break;
                        case generated::Pipeline_Node_Type_IOVideoReader:
                            gen << CodeGen::SetHeader("io/VideoReader");
                            gen << CodeGen::SetClass("saliency_sandbox::io::_VideoReader");
                            gen << CodeGen::SetTemplateArgument("width",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_uint32,RES_WIDTH_S(RESOLUTION) );
                            gen << CodeGen::SetTemplateArgument("height",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_uint32,RES_HEIGHT_S(RESOLUTION));
                            gen << CodeGen::SetConstructorArgument("path",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_string);
                            gen << CodeGen::SetNoInput();
                            break;
                        case generated::Pipeline_Node_Type_IOVideoWriter:
                            gen << CodeGen::SetHeader("io/VideoWriter");
                            gen << CodeGen::SetClass("saliency_sandbox::io::VideoWriter");
                            gen << CodeGen::SetConstructorArgument("path",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_string);
                            gen << CodeGen::SetInput("image",0);
                            break;
                        case generated::Pipeline_Node_Type_UtilsImageConverter:
                            gen << CodeGen::SetHeader("utils/Image");
                            gen << CodeGen::SetClass("saliency_sandbox::utils::_ImageConvert");
                            gen << CodeGen::SetTemplateArgument("width",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_uint32,RES_WIDTH_S(RESOLUTION) );
                            gen << CodeGen::SetTemplateArgument("height",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_uint32,RES_HEIGHT_S(RESOLUTION));
                            gen << CodeGen::SetTemplateArgument("input",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_complex,"saliency_sandbox::utils::_RGBImage");
                            gen << CodeGen::SetTemplateArgument("output",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_complex);
                            gen << CodeGen::SetInput("image",0);
                            break;
                        case generated::Pipeline_Node_Type_SaliencySpectralWhitening:
                            gen << CodeGen::SetHeader("saliency/activation/Spectral");
                            gen << CodeGen::SetClass("saliency_sandbox::saliency::activation::_Spectral");
                            gen << CodeGen::SetTemplateArgument("width",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_uint32,RES_WIDTH_S(RESOLUTION) );
                            gen << CodeGen::SetTemplateArgument("height",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_uint32,RES_HEIGHT_S(RESOLUTION));
                            gen << CodeGen::SetInput("feature",0);
                            break;
                        case generated::Pipeline_Node_Type_SaliencyBooleanMaps:
                            gen << CodeGen::SetHeader("saliency/activation/BooleanMaps");
                            gen << CodeGen::SetClass("saliency_sandbox::saliency::activation::_BooleanMaps");
                            gen << CodeGen::SetTemplateArgument("width",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_uint32,RES_WIDTH_S(RESOLUTION) );
                            gen << CodeGen::SetTemplateArgument("height",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_uint32,RES_HEIGHT_S(RESOLUTION));
                            gen << CodeGen::SetInput("feature",0);
                            break;
                        case generated::Pipeline_Node_Type_SaliencyHighpass:
                            gen << CodeGen::SetHeader("saliency/activation/HighPass");
                            gen << CodeGen::SetClass("saliency_sandbox::saliency::activation::_HighPass");
                            gen << CodeGen::SetTemplateArgument("width",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_uint32,RES_WIDTH_S(RESOLUTION) );
                            gen << CodeGen::SetTemplateArgument("height",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_uint32,RES_HEIGHT_S(RESOLUTION));
                            gen << CodeGen::SetInput("feature",0);
                            break;
                        case generated::Pipeline_Node_Type_SaliencyPearson:
                            gen << CodeGen::SetHeader("saliency/evaluation/Pearson");
                            gen << CodeGen::SetClass("saliency_sandbox::saliency::evaluation::_Pearson");
                            gen << CodeGen::SetTemplateArgument("width",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_uint32,RES_WIDTH_S(RESOLUTION) );
                            gen << CodeGen::SetTemplateArgument("height",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_uint32,RES_HEIGHT_S(RESOLUTION));
                            gen << CodeGen::SetInput("saliency",0);
                            gen << CodeGen::SetInput("fixation",0);
                            break;
                        case generated::Pipeline_Node_Type_SaliencyIttiKoch:
                            gen << CodeGen::SetHeader("saliency/activation/IttiKoch");
                            gen << CodeGen::SetClass("saliency_sandbox::saliency::activation::_IttiKoch");
                            gen << CodeGen::SetTemplateArgument("width",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_uint32,RES_WIDTH_S(RESOLUTION) );
                            gen << CodeGen::SetTemplateArgument("height",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_uint32,RES_HEIGHT_S(RESOLUTION));
                            gen << CodeGen::SetInput("feature",0);
                            break;
                        case generated::Pipeline_Node_Type_SaliencyOrientation:
                            gen << CodeGen::SetHeader("saliency/feature/Orientation");
                            gen << CodeGen::SetClass("saliency_sandbox::saliency::feature::_Orientation");
                            gen << CodeGen::SetTemplateArgument("width",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_uint32,RES_WIDTH_S(RESOLUTION) );
                            gen << CodeGen::SetTemplateArgument("height",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_uint32,RES_HEIGHT_S(RESOLUTION));
                            gen << CodeGen::SetProperty("sigma",generated::Pipeline_Node_Property_Type ::Pipeline_Node_Property_Type_pb_float);
                            gen << CodeGen::SetProperty("theta",generated::Pipeline_Node_Property_Type ::Pipeline_Node_Property_Type_pb_float);
                            gen << CodeGen::SetProperty("lambda",generated::Pipeline_Node_Property_Type ::Pipeline_Node_Property_Type_pb_float);
                            gen << CodeGen::SetProperty("gamma",generated::Pipeline_Node_Property_Type ::Pipeline_Node_Property_Type_pb_float);
                            gen << CodeGen::SetInput("feature",0);
                            break;
                        case generated::Pipeline_Node_Type_SaliencyGBVS:
                            gen << CodeGen::SetHeader("saliency/activation/GBVS");
                            gen << CodeGen::SetClass("saliency_sandbox::saliency::activation::_GBVS");
                            gen << CodeGen::SetTemplateArgument("width",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_uint32,RES_WIDTH_S(RESOLUTION) );
                            gen << CodeGen::SetTemplateArgument("height",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_uint32,RES_HEIGHT_S(RESOLUTION));
                            gen << CodeGen::SetTemplateArgument("iwidth",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_uint32,"40");
                            gen << CodeGen::SetTemplateArgument("iheight",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_uint32,"40");
                            gen << CodeGen::SetProperty("sigma",generated::Pipeline_Node_Property_Type ::Pipeline_Node_Property_Type_pb_float);
                            gen << CodeGen::SetInput("feature",0);
                            break;
                        case generated::Pipeline_Node_Type_SaliencyTracking:
                            gen << CodeGen::SetHeader("saliency/Tracking");
                            gen << CodeGen::SetClass("saliency_sandbox::saliency::_Tracking");
                            gen << CodeGen::SetTemplateArgument("width",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_uint32,RES_WIDTH_S(RESOLUTION) );
                            gen << CodeGen::SetTemplateArgument("height",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_uint32,RES_HEIGHT_S(RESOLUTION));
                            gen << CodeGen::SetInput("feature",0);
                            gen << CodeGen::SetInput("flow",1);
                            break;
                        case generated::Pipeline_Node_Type_UtilsFPSCounter:
                            gen << CodeGen::SetHeader("utils/FPSCounter");
                            gen << CodeGen::SetClass("saliency_sandbox::utils::FPSCounter");
                            gen << CodeGen::SetInput("process",0);
                            break;
                        case generated::Pipeline_Node_Type_UtilsMatrixSplit:
                            gen << CodeGen::SetHeader("utils/Matrix");
                            gen << CodeGen::SetClass("saliency_sandbox::utils::_MatrixSplitt");
                            gen << CodeGen::SetTemplateArgument("width",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_uint32,RES_WIDTH_S(RESOLUTION) );
                            gen << CodeGen::SetTemplateArgument("height",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_uint32,RES_HEIGHT_S(RESOLUTION));
                            gen << CodeGen::SetTemplateArgument("type",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_complex);
                            gen << CodeGen::SetInput("matrix",0);
                            break;
                        case generated::Pipeline_Node_Type_UtilsMatrixMerge:
                            gen << CodeGen::SetHeader("utils/Matrix");
                            gen << CodeGen::SetClass("saliency_sandbox::utils::_MatrixMerge");
                            gen << CodeGen::SetTemplateArgument("width",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_uint32,RES_WIDTH_S(RESOLUTION) );
                            gen << CodeGen::SetTemplateArgument("height",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_uint32,RES_HEIGHT_S(RESOLUTION));
                            gen << CodeGen::SetTemplateArgument("type",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_complex);
                            break;
                        case generated::Pipeline_Node_Type_UtilsMatrixSum:
                            gen << CodeGen::SetHeader("utils/Matrix");
                            gen << CodeGen::SetClass("saliency_sandbox::utils::_MatrixSum");
                            gen << CodeGen::SetTemplateArgument("width",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_uint32,RES_WIDTH_S(RESOLUTION) );
                            gen << CodeGen::SetTemplateArgument("height",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_uint32,RES_HEIGHT_S(RESOLUTION));
                            gen << CodeGen::SetTemplateArgument("type",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_complex);
                            gen << CodeGen::SetInput("matrix",0);
                            break;
                        case generated::Pipeline_Node_Type_UtilsNormalize:
                            gen << CodeGen::SetHeader("utils/Normalize");
                            gen << CodeGen::SetClass("saliency_sandbox::utils::_Normalize");
                            gen << CodeGen::SetTemplateArgument("width",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_uint32,RES_WIDTH_S(RESOLUTION) );
                            gen << CodeGen::SetTemplateArgument("height",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_uint32,RES_HEIGHT_S(RESOLUTION));
                            gen << CodeGen::SetInput("matrix",0);
                            break;
                        case generated::Pipeline_Node_Type_Plot:
                            gen << CodeGen::SetHeader("plot/Plot");
                            gen << CodeGen::SetClass("saliency_sandbox::plot::Plot");
                            gen << CodeGen::SetInput("line 0",0);
                            gen << CodeGen::SetConstructorArgument("line0",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_string,"line0");
                            sserr << sscond(pipeline.node(i).input_size() >= 10) << "the maximum number of input of node \"" << pipeline.node(i).name()
                                  << "\" is 10. found " << pipeline.node(i).input_size() << ssthrow;
                            for(int j = 1; j < pipeline.node(i).input_size() && j < 10; j++) {
                                std::stringstream ss;

                                ss << "line" << j;
                                gen << CodeGen::SetInput(ss.str(), i);
                                gen << CodeGen::SetConstructorArgument(ss.str(),generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_string,ss.str());
                            }
                            break;
                        case generated::Pipeline_Node_Type_PlotQuiver:
                            gen << CodeGen::SetHeader("plot/Quiver");
                            gen << CodeGen::SetClass("saliency_sandbox::plot::Quiver");
                            gen << CodeGen::SetTemplateArgument("width",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_uint32,RES_WIDTH_S(RESOLUTION) );
                            gen << CodeGen::SetTemplateArgument("height",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_uint32,RES_HEIGHT_S(RESOLUTION));
                            gen << CodeGen::SetInput("flow",0);
                            gen << CodeGen::SetInput("rgb",0);
                            break;
                        case generated::Pipeline_Node_Type_OpticalFlow:
                            gen << CodeGen::SetHeader("flow/OpticalFlow");
                            gen << CodeGen::SetClass("saliency_sandbox::flow::_OpticalFlow");
                            gen << CodeGen::SetTemplateArgument("width",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_uint32,RES_WIDTH_S(RESOLUTION) );
                            gen << CodeGen::SetTemplateArgument("height",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_uint32,RES_HEIGHT_S(RESOLUTION));
                            gen << CodeGen::SetInput("next",0);
                            if(pipeline.node(i).input_size() > 1)
                                gen << CodeGen::SetInput("init",1);
                            break;
                        case generated::Pipeline_Node_Type_ExtrapolatePose:
                            gen << CodeGen::SetHeader("flow/Trajectory");
                            gen << CodeGen::SetClass("saliency_sandbox::flow::ExtrapolatePose");
                            gen << CodeGen::SetTemplateArgument("frames",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_uint32,"20");
                            gen << CodeGen::SetInput("pose",0);
                            break;
                        case generated::Pipeline_Node_Type_GazeRandom:
                            gen << CodeGen::SetHeader("gaze/Random");
                            gen << CodeGen::SetClass("saliency_sandbox::gaze::Random");
                            gen << CodeGen::SetNoInput();
                            break;
                        case generated::Pipeline_Node_Type_GazeSelector:
                            gen << CodeGen::SetHeader("gaze/Gaze");
                            gen << CodeGen::SetClass("saliency_sandbox::gaze::Gaze::Selector");
                            gen << CodeGen::SetTemplateArgument("component",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_complex);
                            gen << CodeGen::SetInput("gaze",0);
                            break;
                        case generated::Pipeline_Node_Type_GazeMovementSelector:
                            gen << CodeGen::SetHeader("gaze/GazeMovement");
                            gen << CodeGen::SetClass("saliency_sandbox::gaze::GazeMovement::Selector");
                            gen << CodeGen::SetTemplateArgument("component",generated::Pipeline_Node_Argument_Type::Pipeline_Node_Argument_Type_pb_complex);
                            gen << CodeGen::SetInput("gaze_movement",0);
                            break;
                        case generated::Pipeline_Node_Type_GazeFOV:
                            gen << CodeGen::SetHeader("gaze/FOV");
                            gen << CodeGen::SetClass("saliency_sandbox::gaze::FOV");
                            gen << CodeGen::SetInput("gaze",0);
                            gen << CodeGen::SetInput("depth",1);
                            break;
                        default:
                            sserr << "unknown node type: " << generated::Pipeline_Node::Type_Name(pipeline.node(i).type()) << ssthrow;
                    }
                }
                gen.generate();
                return gen.cpp();
            }
    }
}