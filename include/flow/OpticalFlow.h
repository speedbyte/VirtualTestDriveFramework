//
// Created by geislerd on 15.03.17.
//

#include <flow/Flow.h>

#ifndef OPTICALFLOW_H
#define OPTICALFLOW_H
namespace saliency_sandbox {
    namespace flow {

        template<typename _type>
        class DummyInput : public saliency_sandbox::core::Node::template Input<>::template Output<_type> {
        private:
            _type* m_data_ptr;
        public:
            DummyInput(_type* data_ptr) : m_data_ptr(data_ptr) {
                this->template output<0>()->name("dummy");
                this->template output<0>()->value(this->m_data_ptr);
            }

            void calc() override {

            }

            void reset() override {

            }

            _type* data() {
                return &this->m_data_ptr;
            }
        };

        template<uint32_t _width, uint32_t _height>
        class _OpticalFlow : public _Flow<_width, _height,saliency_sandbox::utils::_IntensityImage<_width,_height>,_FlowField<_width,_height>> {
        private:
            saliency_sandbox::utils::_IntensityImage<_width,_height> m_prev;
            DummyInput<_FlowField<_width,_height>> m_dummy_init;

        public:
            _OpticalFlow() : m_dummy_init(this->template output<0>()->value()) {
                this->template input<0>()->name("next");
                this->template input<1>()->name("init");

                connect_port(this->m_dummy_init,0,*this,1);
            }

            void calc_bak() {
                cv::Mat1b next, prev;
                cv::Mat2f init,flow;
                double pyr_scale, poly_sigma;
                int levels, winsize, iterations, poly_n, flags;

                next = this->template input<0>()->value()->mat();
                prev = this->m_prev.mat();
                init = this->template input<1>()->value()->mat();
                flow = this->flow();

                //cv::GaussianBlur(init,init,cv::Size(31,31),5);

                pyr_scale = this->properties()->template get<float>("pyr_scale",0.5f);
                poly_sigma = this->properties()->template get<float>("poly_sigma",1.1f);

                levels = this->properties()->template get<int>("levels",1);
                winsize = this->properties()->template get<int>("winsize",5);
                iterations = this->properties()->template get<int>("iterations",1);
                poly_n = this->properties()->template get<int>("poly_n",5);
                flags = this->properties()->template get<int>("flags",cv::OPTFLOW_USE_INITIAL_FLOW);

                if(init.data != flow.data)
                    memcpy(flow.data,init.data,_width*_height*sizeof(cv::Vec2f));

                cv::calcOpticalFlowFarneback(prev,next,flow,pyr_scale,levels,winsize,iterations,poly_n,poly_sigma,flags);

                this->m_prev.mat(next);
            }

            void calc() {
                cv::Mat1b next, prev;
                std::vector<cv::Point2f> points[2];
                std::vector<uchar> status;
                std::vector<float> err;
                cv::Vec2f v;
                cv::Mat2f init,flow;

                this->flow(cv::Vec2f(0.0f,0.0f));

                next = this->template input<0>()->value()->mat();
                prev = this->m_prev.mat();
                init = this->template input<1>()->value()->mat();
                flow = this->flow();

                //cv::GaussianBlur(init,init,cv::Size(31,31),5);

                for(cv::Point2f p(0.0f,0.0f); p.x < _width; p.x++) {
                    for(p.y = 0.0f; p.y < _height; p.y++) {
                        v = init.template at<cv::Vec2f>(p);
                        if(v.val[0] < FLT_EPSILON && v.val[1] < FLT_EPSILON)
                            continue;
                        points[0].push_back(p);
                        points[1].push_back(p+cv::Point2f(v));
                    }
                }

                if(this->time() < 1)
                    this->m_prev.mat(next);

                cv::calcOpticalFlowPyrLK(
                        prev,
                        next,
                        points[0],
                        points[1],
                        status,
                        err,
                        cv::Size(3,3),
                        5,
                        cv::TermCriteria(cv::TermCriteria::COUNT+cv::TermCriteria::EPS, 100, 0.01),
                        cv::OPTFLOW_USE_INITIAL_FLOW,
                        1e-4);

                for(int i = 0; i < points[0].size(); i++) {
                    if(!status[i])
                        continue;
                    //if(err[i] > 1.0f)
                    //    continue;
                    std::cout << "p" << i << ": " << err[i] << std::endl;
                    flow.template at<cv::Vec2f>(points[0][i]) = cv::Vec2f(points[1][i].x-points[0][i].x,points[1][i].y-points[0][i].y);
                }

                this->m_prev.mat(next);
            }



            void calc_bak_bak_bak_bak() {
                cv::Mat1b next, prev;
                cv::Mat2f init,flow;
                double pyr_scale, poly_sigma;
                int levels, winsize, iterations, poly_n, flags;

                next = this->template input<0>()->value()->mat();
                prev = this->m_prev.mat();
                init = this->template input<1>()->value()->mat();
                flow = this->flow();

                pyr_scale = this->properties()->template get<float>("pyr_scale",0.5f);
                poly_sigma = this->properties()->template get<float>("poly_sigma",1.1f);

                levels = this->properties()->template get<int>("levels",10);
                winsize = this->properties()->template get<int>("winsize",61);
                iterations = this->properties()->template get<int>("iterations",10);
                poly_n = this->properties()->template get<int>("poly_n",7);
                flags = this->properties()->template get<int>("flags",cv::OPTFLOW_USE_INITIAL_FLOW);


                //cv::GaussianBlur(init,init,cv::Size(31,31),5);

                if(init.data != flow.data)
                    memcpy(flow.data,init.data,_width*_height*sizeof(cv::Vec2f));


                cv::Ptr<cv::DenseOpticalFlow> tvl1 = cv::createOptFlow_DualTVL1();
                tvl1->calc(prev,next,flow);

                cv::calcOpticalFlowFarneback(prev,next,flow,pyr_scale,levels,winsize,iterations,poly_n,poly_sigma,flags);

                this->m_prev.mat(next);
            }

            void reset() override {

            }
        };
    }
}
#endif //OPTICALFLOW_H
