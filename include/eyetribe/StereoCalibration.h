//
// Created by geislerd on 12.04.17.
//

#ifndef EYETRIBE_STEREOCALIBRATION_H
#define EYETRIBE_STEREOCALIBRATION_H

#include <eyetribe/VideoReader.h>
#include <vector>

namespace saliency_sandbox {
    namespace eyetribe {

        template <uint32_t _format>
        class StereoCalibration : public saliency_sandbox::core::Node::
            template Input<typename VideoReader<_format>::Image,typename VideoReader<_format>::Image>::
            template Output<typename VideoReader<_format>::Image,typename VideoReader<_format>::Image> {
        public:
            StereoCalibration() {
                this->template input<0>()->name("image0");
                this->template input<1>()->name("image1");
            }

            void calc() override {
                cv::Mat1b input[2], scaled[2];
                std::vector<cv::Vec2f> corners[2];
                bool found[2];
                cv::Size sz;
                float scale;

                sz = cv::Size(4,4);
                scale = 0.15;

                input[0] = this->template input<0>()->value()->mat();
                input[1] = this->template input<1>()->value()->mat();

                cv::resize(input[0],scaled[0],cv::Size(),scale,scale);
                cv::resize(input[1],scaled[1],cv::Size(),scale,scale);

                found[0] = cv::findChessboardCorners(scaled[0],sz,corners[0]);
                found[1] = cv::findChessboardCorners(scaled[1],sz,corners[1]);

                if(!found[0] || !found[1])
                    return;

                for(int i = 0; i < corners[0].size(); corners[0][i++] *= 1.0f/scale);
                for(int i = 0; i < corners[1].size(); corners[1][i++] *= 1.0f/scale);

                cv::cornerSubPix(input[0],corners[0],sz,cv::Size(-1,-1),cv::TermCriteria(cv::TermCriteria::COUNT+cv::TermCriteria::EPS, 30, 0.01));

                cv::drawChessboardCorners(input[0],sz,corners[0],found[0]);
                cv::drawChessboardCorners(input[1],sz,corners[1],found[1]);

            }

            void reset() override {

            }
        };
    }
}

#endif //EYETRIBE_STEREOCALIBRATION_H
