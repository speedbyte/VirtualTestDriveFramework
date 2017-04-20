//
// Created by geislerd on 19.04.17.
//

#ifndef EYETRIBE_STEREOGLINTMATCHER_H
#define EYETRIBE_STEREOGLINTMATCHER_H

#include <eyetribe/GlintDetector.h>
#include <eyetribe/StereoRectification.h>
#include <boost/lexical_cast.hpp>

namespace saliency_sandbox {
    namespace eyetribe {

        typedef std::pair<int,Glint> GlintSort;
        typedef std::vector<GlintSort> GlintSortList;

        bool glint_comp(GlintSort i , GlintSort j) {
            return i.second.first.x < j.second.first.x;
        }

        template<uint32_t _format>
        class StereoGlintMatcher : public saliency_sandbox::core::Node::
        template Input<
                typename Format<_format>::Image,
                typename Format<_format>::Image,
                GlintList,
                GlintList,
                RectificationProjection,
                RectificationProjection
        >::template Output<
                GlintList,
                GlintList
        > {
        private:
            GlintList m_glints[2];
        public:
            StereoGlintMatcher() {
                this->template input<0>()->name("image0");
                this->template input<1>()->name("image1");
                this->template input<2>()->name("glints0");
                this->template input<3>()->name("glints1");
                this->template input<4>()->name("P0");
                this->template input<5>()->name("P1");

                this->template output<0>()->name("glints0");
                this->template output<0>()->value(&this->m_glints[0]);
                this->template output<1>()->name("glints1");
                this->template output<1>()->value(&this->m_glints[1]);
            }

            float mad(const Glint& glint0, const Glint& glint1, const cv::Mat1b& in0, const cv::Mat1b& in1) {
                int o = 25;
                float mad;

                cv::Rect ir0(0,0,in0.cols,in0.rows);
                cv::Rect ir1(0,0,in0.cols,in0.rows);
                cv::Rect r0(glint0.first.x-o,glint0.first.y-0,o*2,o*2);
                cv::Rect r1(glint1.first.x-o,glint1.first.y-0,o*2,o*2);

                cv::Mat1b patch0 = in0(ir0 & r0);
                cv::Mat1b patch1 = in1(ir1 & r1);

                for(int i = 0; i < patch0.cols*patch0.rows;i++)
                    for(int j = 0; j < patch1.cols*patch1.rows;j++)
                        mad += fabsf(patch0.template at<uchar>(i)-patch1.template at<uchar>(j));

                return mad/float(patch0.rows*patch0.cols*patch1.rows*patch1.cols);
            }

            float brd(const Glint& glint0, const Glint& glint1, const cv::Mat1b& in0, const cv::Mat1b& in1) {
                int o = 25;
                float mad;

                cv::Rect ir0(0,0,in0.cols,in0.rows);
                cv::Rect ir1(0,0,in0.cols,in0.rows);
                cv::Rect r0(glint0.first.x-o,glint0.first.y-0,o*2,o*2);
                cv::Rect r1(glint1.first.x-o,glint1.first.y-0,o*2,o*2);

                r0 &= ir0;
                r1 &= ir1;

                if(r0.width != r1.width || r0.height != r1.height)
                    return 1000000;

                cv::Mat1b patch0 = in0(r0);
                cv::Mat1b patch1 = in1(r1);

                cv::Mat1b d[2][2], diff[2];

                cv::reduce(patch0,d[0][0],0,CV_REDUCE_AVG);
                cv::reduce(patch0,d[0][1],1,CV_REDUCE_AVG);
                cv::reduce(patch1,d[1][0],0,CV_REDUCE_AVG);
                cv::reduce(patch1,d[1][1],1,CV_REDUCE_AVG);

                cv::absdiff(d[0][0],d[1][0],diff[0]);
                cv::absdiff(d[0][1],d[1][1],diff[1]);

                return float((cv::sum(diff[0])+cv::sum(diff[1])).val[0]);
            }

            float dist(const Glint& glint0, const Glint& glint1, const cv::Mat1b& in0, const cv::Mat1b& in1) {
                std::vector<float> dists;
                float dist;

                dist = 0;

                //dists.push_back(glint0.second - glint1.second);
                //dists.push_back(cv::norm(glint0.first-glint1.first));
                //dists.push_back(this->mad(glint0,glint1,in0,in1));
                dists.push_back(this->brd(glint0,glint1,in0,in1));

                for(int i = 0; i < dists.size(); i++)
                    dist += dists[i]*dists[i];

                return sqrtf(dist);
            }

            cv::Mat1f dist(const GlintList &glints0, const GlintList &glints1, const cv::Mat1b& in0, const cv::Mat1b& in1) {
                cv::Mat1f dist;

                dist = cv::Mat1f(int(glints0.size()),int(glints1.size()));
                for(int i0 = 0; i0 < glints0.size(); i0++)
                    for(int i1 = 0; i1 < glints1.size(); i1++)
                        dist.template at<float>(i0,i1) = this->dist(glints0[i0],glints1[i1],in0,in1);


                /*cv::normalize(dist,dist,0,1,cv::NORM_MINMAX);
                cv::resize(dist,dist,cv::Size(),10,10,cv::INTER_NEAREST);
                cv::namedWindow("DEBUG GLINT DIST");
                cv::imshow("DEBUG GLINT DIST",dist);*/

                return dist;
            }

            void sort() {
                GlintSortList v;
                size_t sz;
                GlintList glints[2];

                glints[0] = this->m_glints[0];
                glints[1] = this->m_glints[1];

                sz = this->m_glints[0].size();

                for(int i = 0; i < sz; i++)
                    v.push_back(GlintSort(i,glints[0][i]));

                std::sort(v.begin(),v.end(),glint_comp);

                for(int i = 0; i < sz; i++) {
                    this->m_glints[0][i] = glints[0][v[i].first];
                    this->m_glints[1][i] = glints[1][v[i].first];
                }
            }

            void calc() override {
                cv::Mat1b in0 = this->template input<0>()->value()->mat();
                cv::Mat1b in1 = this->template input<1>()->value()->mat();
                const GlintList& glints0 = *this->template input<2>()->value();
                const GlintList& glints1 = *this->template input<3>()->value();
                const RectificationProjection& p0 = *this->template input<4>()->value();
                const RectificationProjection& p1 = *this->template input<5>()->value();
                cv::Mat1f dist;
                size_t minsz;
                cv::Point min;
                double maxv;

                if(glints0.empty())
                    return;

                if(glints1.empty())
                    return;

                minsz = MIN(glints0.size(),glints1.size());
                dist = this->dist(glints0,glints1,in0,in1);

                this->m_glints[0].clear();
                this->m_glints[1].clear();

                for(int i = 0; i < minsz; i++) {
                    cv::minMaxLoc(dist, nullptr, &maxv, &min, nullptr);
                    this->m_glints[0].push_back(glints0[min.y]);
                    this->m_glints[1].push_back(glints1[min.x]);
                    dist.row(min.y) = maxv*2;
                    dist.col(min.x) = maxv*2;
                }

                this->sort();

                /*
                for(int i = 0; i < this->m_glints[0].size(); i++)
                    cv::circle(in0,this->m_glints[0][i].first,10,cv::Scalar::all(255));
                for(int i = 0; i < this->m_glints[1].size(); i++)
                    cv::circle(in1,this->m_glints[1][i].first,10,cv::Scalar::all(255));

                for(int i = 0; i < minsz; i++) {
                    cv::putText(in0,boost::lexical_cast<std::string>(i),this->m_glints[0][i].first,CV_FONT_HERSHEY_SIMPLEX,1,cv::Scalar::all(255));
                    cv::putText(in1,boost::lexical_cast<std::string>(i),this->m_glints[1][i].first,CV_FONT_HERSHEY_SIMPLEX,1,cv::Scalar::all(255));
                }
                 */
            }

            void reset() override {

            }
        };
    }
}

#endif //EYETRIBE_STEREOGLINTMATCHER_H
