//
// Created by geislerd on 19.04.17.
//

#ifndef EYETRIBE_GLINTDETECTOR_H
#define EYETRIBE_GLINTDETECTOR_H

#include <eyetribe/VideoReader.h>

namespace saliency_sandbox {
    namespace eyetribe {

        typedef saliency_sandbox::utils::_Matrix<31,31,float> GlintKernel;
        typedef std::pair<cv::Point,double> Glint;
        typedef std::vector<Glint> GlintList;

        template<uint32_t _format>
        class GlintDetector : public saliency_sandbox::core::Node::
        template Input<typename Format<_format>::Image>::template Output<GlintList> {
        private:
            GlintList m_glints;
            GlintKernel m_kernel;

        public:
            GlintDetector() {
                this->template input<0>()->name("image");

                this->template output<0>()->name("glints");
                this->template output<0>()->value(&this->m_glints);

                this->reset();
            }

            void calc() override {
                cv::Mat1b in = this->template input<0>()->value()->mat();
                const int sz = this->properties()->template get<int>("glint_size",71);
                const cv::Rect ir(0,0,in.cols,in.rows);
                cv::Mat1f t, r[3], k;
                Glint tmp;
                cv::Rect reg;
                cv::Mat1b thresh, l[10];

                this->m_glints.clear();


                cv::GaussianBlur(in,l[0],cv::Size(13,13),0.20f);
                cv::GaussianBlur(in,l[1],cv::Size(13,13),2.00f);
                cv::subtract(l[0],l[1],l[3]);
                cv::threshold(l[3],l[4],cv::mean(l[3]).val[0]*100,255,cv::THRESH_BINARY);
                cv::GaussianBlur(l[4],l[5],cv::Size(sz,sz),4.0f);
                //cv::Mat1f tt; cv::normalize(l[5],tt,0.0f,1.0f,cv::NORM_MINMAX,CV_32FC1);cv::namedWindow("AA");cv::imshow("AA",tt);

                for(int i = 0 ; i < 100; i++) {
                    cv::minMaxLoc(l[5], nullptr,&tmp.second, nullptr,&tmp.first);
                    if(tmp.second < FLT_MIN)
                        break;
                    this->m_glints.push_back(tmp);
                    l[5](ir & cv::Rect(tmp.first.x-sz,tmp.first.y-sz,sz*2,sz*2)) = 0;
                }
            }

            void reset() override {
                this->m_glints.clear();

                this->m_kernel.clear();
                cv::subtract(this->m_kernel,-1,this->m_kernel);
                this->m_kernel.template at<float>((this->m_kernel.rows-1)/2,(this->m_kernel.cols-1)/2) = (this->m_kernel.rows*this->m_kernel.cols -1);
            }
        };
    }
}
#endif //EYETRIBE_GLINTDETECTOR_H
