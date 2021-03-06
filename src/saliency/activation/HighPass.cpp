//
// Created by geislerd on 10.03.17.
//

#include <saliency/activation/HighPass.h>

namespace saliency_sandbox {
    namespace saliency {
        namespace activation {

            void _HighPassImpl::highpass(cv::Mat1f in, cv::Mat1f out, cv::Size sz, float sigma) {
                cv::Mat1f smooth;

                cv::GaussianBlur(in,smooth,sz,sigma,sigma,cv::BORDER_REPLICATE);
                cv::absdiff(in,smooth,out);
            }
        }
    }
}