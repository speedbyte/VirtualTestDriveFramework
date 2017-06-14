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
            cv::Ptr<cv::DenseOpticalFlow> m_tvl1;

        public:
            _OpticalFlow() :
                    m_dummy_init(this->template output<0>()->value()),
                    m_tvl1(cv::createOptFlow_DualTVL1()) {
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

            void calc_bak_bak_bak_bak() {
                return;
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


            void calc_() {
                cv::Mat1b next, prev;
                cv::Mat2f init,flow;
                double pyr_scale, poly_sigma;

                next = this->template input<0>()->value()->mat();
                prev = this->m_prev.mat();
                init = this->template input<1>()->value()->mat();
                flow = this->flow();

                cv::blur(next,next,cv::Size(100,100));

                if(init.data != flow.data)
                    memcpy(flow.data,init.data,_width*_height*sizeof(cv::Vec2f));

                cv::Mat2f tmp(_height,_width);
                for(cv::Point p(0,0); p.x < _width; p.x++) {
                    for(p.y = 0; p.y < _height; p.y++) {
                        cv::Vec2f f = flow.template at<cv::Vec2f>(p);
                        cv::Point q = p - cv::Point((int)f.val[0],(int)f.val[1]);
                        if(!cv::Rect(0,0,_width,_height).contains(q))
                            continue;
                        flow.template at<cv::Vec2f>(p) *= fabsf(prev.template at<uchar>(q)-next.template at<uchar>(p)); //cv::Vec2f(prev.template at<uchar>(q)-next.template at<uchar>(p),next.template at<uchar>(p)-prev.template at<uchar>(q));
                    }
                }

                this->m_prev.mat(next);
            }

            void calc() {
                return;
                cv::KalmanFilter kf;
                cv::Mat2f init,flow;
                cv::Mat1b next, prev;

                kf.init(33*33,2);
                cv::setIdentity(kf.measurementMatrix);
                cv::setIdentity(kf.processNoiseCov, cv::Scalar::all(1e-4));
                cv::setIdentity(kf.measurementNoiseCov, cv::Scalar::all(10));
                cv::setIdentity(kf.errorCovPost, cv::Scalar::all(.1));

                next = this->template input<0>()->value()->mat();
                prev = next;//this->m_prev.mat();
                init = this->template input<1>()->value()->mat();
                flow = this->flow();

                for(cv::Point p(16,16); p.x < _width; p.x+=16) {
                    for(p.y = 16; p.y < _height; p.y+=16) {
                        cv::Rect patchRectNext = cv::Rect(p.x-16,p.y-16,33,33);
                        cv::Mat1f patchNext = next(patchRectNext);
                        patchNext.rows *= patchNext.cols;
                        patchNext.cols = 1;

                        cv::Vec2f f = init.template at<cv::Vec2f>(p);
                        cv::Point q = p - cv::Point(f);
                        cv::Rect patchRectPrev = cv::Rect(q.x-16,q.y-16,33,33);
                        cv::Mat1f patchPrev = prev(patchRectPrev);
                        patchPrev.cols *= patchPrev.rows;
                        patchPrev.rows = 1;

                        cv::Mat1f diff;
                        cv::absdiff(cv::repeat(patchNext,patchNext.cols,patchNext.rows),cv::repeat(patchPrev,patchPrev.cols,patchPrev.rows),diff);

                        cv::Point minDiff;
                        cv::minMaxLoc(diff, nullptr, nullptr, &minDiff);

                        std::cout << minDiff << std::endl;
                    }
                }
            }

            void calc__() {
                cv::Mat1b next, prev;
                cv::Mat2f init,flow;
                double pyr_scale, poly_sigma;
                int levels, winsize, iterations, poly_n, flags;

                next = this->template input<0>()->value()->mat();
                prev = this->m_prev.mat();
                init = this->template input<1>()->value()->mat();
                flow = this->flow();

                // parameter, specifying the image scale (<1) to build pyramids for each image; pyr_scale=0.5 means a
                // classical pyramid, where each next layer is twice smaller than the previous one.
                pyr_scale = this->properties()->template get<float>("pyr_scale",0.5f);

                // standard deviation of the Gaussian that is used to smooth derivatives used as a basis for the
                // polynomial expansion; for poly_n=5, you can set poly_sigma=1.1, for poly_n=7, a good value would be
                // poly_sigma=1.5.
                poly_sigma = this->properties()->template get<float>("poly_sigma",2.0f);

                //  number of pyramid layers including the initial image; levels=1 means that no extra layers are
                // created and only the original images are used.
                levels = this->properties()->template get<int>("levels",1);

                // averaging window size; larger values increase the algorithm robustness to image noise and give more
                // chances for fast motion detection, but yield more blurred motion field.
                winsize = this->properties()->template get<int>("winsize",51);

                // number of iterations the algorithm does at each pyramid level.
                iterations = this->properties()->template get<int>("iterations",10);

                // size of the pixel neighborhood used to find polynomial expansion in each pixel; larger values mean
                // that the image will be approximated with smoother surfaces, yielding more robust algorithm and more
                // blurred motion field, typically poly_n =5 or 7.
                poly_n = this->properties()->template get<int>("poly_n",20);

                flags = this->properties()->template get<int>("flags",cv::OPTFLOW_USE_INITIAL_FLOW);


                //cv::GaussianBlur(init,init,cv::Size(31,31),5);

                if(init.data != flow.data)
                    memcpy(flow.data,init.data,_width*_height*sizeof(cv::Vec2f));

                // Time step of the numerical scheme
                this->m_tvl1->setDouble("tau",this->properties()->template get<float>("tau",1.0f));

                // Weight parameter for the data term, attachment parameter. This is the most relevant parameter,
                // which determines the smoothness of the output. The smaller this parameter is,
                // the smoother the solutions we obtain. It depends on the range of motions of the images,
                // so its value should be adapted to each image sequence.
                this->m_tvl1->setDouble("lambda",this->properties()->template get<float>("lambda",0.1f));

                // Weight parameter for (u - v)^2, tightness parameter. It serves as a link between the attachment
                // and the regularization terms. In theory, it should have a small value in order to maintain both
                // parts in correspondence. The method is stable for a large range of values of this parameter.
                this->m_tvl1->setDouble("theta",this->properties()->template get<float>("theta",0.95f));

                // Number of scales used to create the pyramid of images.
                this->m_tvl1->setDouble("nscales",this->properties()->template get<int>("nscales",5));

                // Number of warpings per scale. Represents the number of times that I1(x+u0) and grad( I1(x+u0) )
                // are computed per scale. This is a parameter that assures the stability of the method. It also
                // affects the running time, so it is a compromise between speed and accuracy.
                this->m_tvl1->setDouble("warps",this->properties()->template get<int>("warps",5));

                // Stopping criterion threshold used in the numerical scheme, which is a trade-off between precision
                // and running time. A small value will yield more accurate solutions at the expense of a slower
                // convergence.
                this->m_tvl1->setDouble("epsilon",this->properties()->template get<float>("epsilon",0.1f));

                // Stopping criterion iterations number used in the numerical scheme.
                this->m_tvl1->setDouble("iterations",this->properties()->template get<int>("iterations",5));

                // Calculate TVL1
                //this->m_tvl1->calc(prev,next,flow);

                //cv::calcOpticalFlowSF(prev,next,flow,3, 2, 4, 4.1, 25.5, 18, 55.0, 25.5, 0.35, 18, 55.0, 25.5, 10);

                cv::calcOpticalFlowFarneback(prev,next,flow,pyr_scale,levels,winsize,iterations,poly_n,poly_sigma,flags);
                //cv::subtract(flow,init,flow);

                this->m_prev.mat(next);
            }

            void reset() override {

            }
        };
    }
}
#endif //OPTICALFLOW_H
