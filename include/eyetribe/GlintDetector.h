//
// Created by geislerd on 19.04.17.
//

#ifndef EYETRIBE_GLINTDETECTOR_H
#define EYETRIBE_GLINTDETECTOR_H

#include <eyetribe/VideoReader.h>

namespace saliency_sandbox {
    namespace eyetribe {

        typedef std::pair<cv::Point,double> Glint;
        typedef std::vector<Glint> GlintList;

        template<uint32_t _format>
        class GlintDetector : public saliency_sandbox::core::Node::
        template Input<typename Format<_format>::Image>::template Output<GlintList> {
        private:
            GlintList m_glints;

        public:
            GlintDetector() {
                this->template input<0>()->name("image");

                this->template output<0>()->name("glints");
                this->template output<0>()->value(&this->m_glints);

                this->reset();
            }

#define dshow( name , I ) do {\
            cv::Mat1f name##_tmp_d; \
            cv::normalize(I,name##_tmp_d,0.0f,1.0f,cv::NORM_MINMAX,CV_32FC1); \
            cv::namedWindow(#name); \
            cv::imshow(#name,I); \
            } while(false);

            float thresh(const cv::Mat1b &in, cv::Mat1b &thresh, int glint_size) {
                double meanVal, maxVal, threshVal;

                minMaxLoc(in, nullptr, &maxVal, nullptr, nullptr);
                meanVal = mean(in).val[0];
                threshVal = maxVal*0.15 + meanVal*0.85;

                adaptiveThreshold(in, thresh, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, glint_size, -threshVal);

                return float(threshVal);
            }

            void calc() override {
                cv::Mat1b in, thresh;
                int sz;
                cv::Rect ir, reg;
                Glint tmp;
                double th;


                this->m_glints.clear();

                in = this->template input<0>()->value()->mat();
                sz = this->properties()->template get<int>("glint_size",51);
                ir = cv::Rect(0,0,in.cols,in.rows);
                th = this->thresh(in, thresh, sz);

                /*
                cv::Mat1b p;
                cv::pyrDown(in,p);
                std::vector<cv::Point2f> ps;
                if(cv::findCirclesGridDefault(p,cv::Size(8,8),ps)) {
                    tmp.first = ps[0]*2;
                    this->m_glints.push_back(tmp);
                    tmp.first = ps[ps.size()-1]*2;
                    this->m_glints.push_back(tmp);
                }
                return;
*/

                for(int i = 0; i < 16; i++) {
                    cv::minMaxLoc(in, nullptr, &tmp.second, nullptr, &tmp.first,thresh);

                    if(tmp.first.x <= 0 || tmp.first.y <= 0 || tmp.second < th)
                        break;

                    reg = ir & cv::Rect(tmp.first.x-sz,tmp.first.y-sz,sz*2,sz*2);

                    tmp.second -= cv::mean(in(reg),thresh(reg)).val[0];

                    thresh(reg) = 0;

                    this->m_glints.push_back(tmp);
                }

                std::sort(this->m_glints.begin(),this->m_glints.end(),[](const Glint& g0, const Glint& g1){ return g0.second > g1.second;});
                while(this->m_glints.size() > 4)
                    this->m_glints.pop_back();
            }

            void reset() override {
            }
        };
    }
}
#endif //EYETRIBE_GLINTDETECTOR_H
