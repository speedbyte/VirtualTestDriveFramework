//
// Created by geislerd on 15.03.17.
//

#include <utils/Image.h>

#ifndef FLOW_H
#define FLOW_H
namespace saliency_sandbox {
    namespace flow {

        template<uint32_t _width, uint32_t _height, uint32_t _depth>
        class _FlowField : saliency_sandbox::utils::_Matrix<_width, _height, cv::Vec<float,_depth>> {
        public:
            typedef cv::Vec<float,_depth> FlowVec;
        };

        template<uint32_t _width, uint32_t _height>
        class _FlowField<_width,_height,2> : saliency_sandbox::utils::_Matrix<_width, _height, cv::Vec2f> {
        public:
            typedef cv::Vec2f FlowVec;
        };

        template<uint32_t _width, uint32_t _height>
        class _FlowField<_width,_height,3> : saliency_sandbox::utils::_Matrix<_width, _height, cv::Vec3f> {
        public:
            typedef cv::Vec3f FlowVec;
        };

        template<uint32_t _width, uint32_t _height, uint32_t _depth, typename... _input>
        class _Flow : public core::Node::template Input<_input...>::template Output<_FlowField<_width,_height,_depth>> {
        public:
            typedef _FlowField<_width, _height,_depth> FlowField;

        private:
            FlowField m_flow;

        public:
            _Flow() {
                this->template output<0>()->name("flow");
                this->template output<0>()->value(&(this->m_flow));
            };

        protected:

            cv::Mat_<FlowField::FlowVec> flow() {
                    return this->m_flow.mat();
            }
        };
    }
}
#endif //FLOW_H
