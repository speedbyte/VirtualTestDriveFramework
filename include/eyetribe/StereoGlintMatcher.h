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

        typedef struct {
            Glint left;
            Glint right;
            size_t left_id;
            size_t right_id;
            cv::Point3f world;
        } StereoGlint;
        typedef std::vector<StereoGlint> StereoGlintList;

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
                RectificationProjection,
                RectificationDisparity
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

                mad /= float(patch0.rows*patch0.cols*patch1.rows*patch1.cols);

                if(isnanf(mad))
                    return FLT_MIN;

                return mad;
            }

            float brd(const Glint& glint0, const Glint& glint1, const cv::Mat1b& in0, const cv::Mat1b& in1) {
                int o = 11;
                float mad;

                cv::Rect ir0(0,0,in0.cols,in0.rows);
                cv::Rect ir1(0,0,in1.cols,in1.rows);
                cv::Rect r0(glint0.first.x-o,glint0.first.y-0,o*2+1,o*2+1);
                cv::Rect r1(glint1.first.x-o,glint1.first.y-0,o*2+1,o*2+1);

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

                return float((cv::sum(diff[0])+cv::sum(diff[1])).val[0]/o);
            }

            float dist(const Glint& glint0, const Glint& glint1, const cv::Mat1b& in0, const cv::Mat1b& in1) {
                std::vector<float> dists;
                float dist;

                dist = 0;

                cv::Point center(Format<_format>::WIDTH/2,Format<_format>::HEIGHT/2);

                //dists.push_back(glint0.second - glint1.second);

                dists.push_back(cv::norm((glint0.first-center)-(glint1.first-center)));
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

            void triangulatePoints(StereoGlintList &glints) {
                const RectificationProjection& p0 = *this->template input<4>()->value();
                const RectificationProjection& p1 = *this->template input<5>()->value();
                const RectificationDisparity& Q = *this->template input<6>()->value();
                const size_t glints_size = glints.size();
                cv::Mat1f left, right;
                cv::Mat1f world;
                float scale;
                cv::Vec4f h, hq;

                left = cv::Mat1f(2,int(glints_size));
                right = cv::Mat1f(2,int(glints_size));
                world = cv::Mat1f(4,int(glints_size));

                for(int i = 0; i < glints_size; i++) {
                    left.template at<float>(0,i) = glints[i].left.first.x;
                    left.template at<float>(1,i) = glints[i].left.first.y;
                    right.template at<float>(0,i) = glints[i].right.first.x;
                    right.template at<float>(1,i) = glints[i].right.first.y;
                }

                cv::triangulatePoints(p0,p1,left,right,world);

                for(int i = 0; i < glints_size; i++) {
                    scale = world.template at<float>(3,i);
                    glints[i].world.x = world.template at<float>(0,i) / scale;
                    glints[i].world.y = world.template at<float>(1,i) / (scale*2);
                    glints[i].world.z = world.template at<float>(2,i) / scale;
                }
            }

            void triangulatePoints(const GlintList &left, const GlintList &right, StereoGlintList& glints) {
                const size_t left_size = left.size();
                const size_t right_size = right.size();
                StereoGlint tmp;

                for(size_t i = 0; i < left_size; i++)
                    for(size_t j = 0; j < left_size; j++) {
                        tmp.left = left[i];
                        tmp.right = right[j];
                        tmp.left_id = i;
                        tmp.right_id = j;
                        tmp.world = cv::Point3f();
                        glints.push_back(tmp);
                    }

                this->triangulatePoints(glints);
            }

            void calc() override {
                cv::Mat1b in0 = this->template input<0>()->value()->mat();
                cv::Mat1b in1 = this->template input<1>()->value()->mat();
                const GlintList& glints0 = *this->template input<2>()->value();
                const GlintList& glints1 = *this->template input<3>()->value();
                cv::Mat1f dist;
                size_t minsz;
                cv::Point min;
                double maxv, minv;

                StereoGlintList stereoGlints;

                if(glints0.empty())
                    return;

                if(glints1.empty())
                    return;

                this->triangulatePoints(glints0,glints1,stereoGlints);
/*
                for(int i = 0; i < stereoGlints.size(); i++) {
                    if(stereoGlints[i].left_id != stereoGlints[i].right_id)
                        continue;
                    //if(stereoGlints[i].world.z > 50 && stereoGlints[i].world.z < 300)
                        std::cout << "i: " << i << "    " << stereoGlints[i].world << std::endl;
                }
                std::cout << std::endl;

                std::cout << "dist x: " << fabsf(stereoGlints[0].world.x-stereoGlints[3].world.x) << std::endl;
                std::cout << "dist y: " << fabsf(stereoGlints[0].world.y-stereoGlints[3].world.y) << std::endl;
                std::cout << "dist z: " << fabsf(stereoGlints[0].world.z-stereoGlints[3].world.z) << std::endl;
                std::cout << "dist xyz: " << cv::norm(stereoGlints[0].world-stereoGlints[3].world) << std::endl << std::endl;
                return;
                std::vector<cv::Vec2d> g0, g1;
                std::vector<cv::Vec4d> g01;

                cv::Mat1f mglints0, mglints1, mglints01;
                size_t sz, k;
                cv::Matx34f mp0, mp1;

                sz = glints0.size()*glints1.size();
                k = 0;
                mglints0 = cv::Mat1f(2,sz);
                mglints1 = cv::Mat1f(2,sz);
                mglints01 = cv::Mat1f(4,sz);

                for(int i = 0; i < glints0.size(); i++)
                    for(int j = 0; j < glints1.size(); j++) {
                        mglints0.template at<float>(0,k) = glints0[i].first.x;
                        mglints0.template at<float>(1,k) = glints0[i].first.y;
                        mglints1.template at<float>(0,k) = glints1[j].first.x;
                        mglints1.template at<float>(1,k) = glints1[j].first.y;
                        k++;
                    }

                cv::triangulatePoints(mp0,mp1,mglints0,mglints1,mglints01);
                mglints01 = mglints01.t();

                cv::Mat3f mglints013D(sz,1);
                cv::convertPointsFromHomogeneous(cv::Mat4f(1,sz,(cv::Vec4f*)mglints01.data),mglints013D);

                std::cout << std::endl << mglints013D << std::endl;


                minsz = MIN(glints0.size(),glints1.size());
                dist = this->dist(glints0,glints1,in0,in1);

                this->m_glints[0].clear();
                this->m_glints[1].clear();

                for(int i = 0; i < minsz; i++) {
                    cv::minMaxLoc(dist, &minv, &maxv, &min, nullptr);
                    this->m_glints[0].push_back(glints0[min.y]);
                    this->m_glints[1].push_back(glints1[min.x]);
                    dist.row(min.y) = maxv*2;
                    dist.col(min.x) = maxv*2;
                }

                this->sort();

*/
                cv::Mat1b I[2];

                in0.copyTo(I[0]);
                in1.copyTo(I[1]);

                for(int i = 0; i < glints0.size(); i++)
                    cv::circle(I[0],glints0[i].first,10,cv::Scalar::all(255));
                for(int i = 0; i < glints1.size(); i++)
                    cv::circle(I[1],glints1[i].first,10,cv::Scalar::all(255));
/*
                for(int i = 0; i < minsz; i++) {
                    cv::putText(I[0],boost::lexical_cast<std::string>(i),this->m_glints[0][i].first,CV_FONT_HERSHEY_SIMPLEX,1,cv::Scalar::all(255));
                    cv::putText(I[1],boost::lexical_cast<std::string>(i),this->m_glints[1][i].first,CV_FONT_HERSHEY_SIMPLEX,1,cv::Scalar::all(255));
                }
                */

                dshow(AA,I[0]);
                dshow(BB,I[1]);

            }

            void reset() override {

            }
        };
    }
}

#endif //EYETRIBE_STEREOGLINTMATCHER_H
