//
// Created by geislerd on 18.04.17.
//

#ifndef EYETRIBE_COLLECTCALIBRATIONCORNERS_H
#define EYETRIBE_COLLECTCALIBRATIONCORNERS_H

#include <eyetribe/DetectCalibrationCorners.h>

namespace saliency_sandbox {
    namespace eyetribe {
        typedef std::vector<CameraCorners> CameraCornersList;
        typedef std::vector<ObjectCorners> ObjectCornersList;

        template <uint32_t _format>
        class CollectCalibrationCornersImpl {
        private:
            typedef Format<_format> format_t;
            saliency_sandbox::utils::_HeatmapImage<Format<_format>::WIDTH/10, Format<_format>::HEIGHT/10> m_density;
        protected:
            CameraCornersList m_camera_corners;
            ObjectCornersList m_object_corners;
            bool m_valid;

            cv::Point mean(CameraCorners &corners) {
                cv::Point mean;
                size_t sz;

                mean = cv::Point(0,0);
                sz = corners.size();

                for(int i = 0; i < sz; i++) {
                    mean.x += corners[i].val[0] / corners.size();
                    mean.y += corners[i].val[1] / corners.size();
                }

                return mean;
            }

            int findCornerToDelete() {
                cv::Point maxLoc, mean;
                std::vector<cv::Point> mean_list;
                std::vector<float> dist_list;
                int idx;
                float min_dist, tmp_dist;

                this->m_density.clear();
                for(int i = 0; i < this->m_camera_corners.size(); i++) {
                    mean = this->mean(this->m_camera_corners[i]);
                    mean_list.push_back(mean);
                    this->m_density.template at<float>(mean*0.1) += 1.0f;
                }

                cv::GaussianBlur(this->m_density,this->m_density,cv::Size(33,33),5.0f);
                cv::normalize(this->m_density,this->m_density,0,1,cv::NORM_MINMAX);
                cv::namedWindow("DENSITY");
                cv::imshow("DENSITY",this->m_density);

                cv::minMaxLoc(this->m_density, nullptr, nullptr, nullptr, &maxLoc);
                maxLoc *= 10.0f;

                min_dist = FLT_MAX;
                idx = -1;
                for(int i = 0; i < mean_list.size(); i++) {
                    tmp_dist = float(cv::norm(mean_list[i]-maxLoc));
                    if(tmp_dist < min_dist) {
                        min_dist = tmp_dist;
                        idx = i;
                    }

                }

                return idx;
            }

            float coverage() {
                cv::Mat1f thresh;
                cv::threshold(this->m_density,thresh,0.1,1.0,cv::THRESH_BINARY);

                return float(cv::sum(thresh).val[0] / (thresh.cols*thresh.rows));
            }

            void collect(CameraCorners* camera_corners, ObjectCorners* objct_corners, bool* valid, saliency_sandbox::core::PropertyMap* pm) {
                int max, idx;
                float coverage, min_coverage;

                if(!*valid)
                    return;

                if(this->m_valid)
                    return;

                max = pm->template get<int>("max",50);
                min_coverage = pm->template get<float>("coverage",0.75f);

                if(this->m_camera_corners.size() > max) {
                    idx = this->findCornerToDelete();
                    this->m_camera_corners[idx] = *camera_corners;
                    this->m_object_corners[idx] = *objct_corners;
                    coverage = this->coverage();
                    std::cout << coverage << std::endl;
                    this->m_valid = coverage > min_coverage;
                } else {
                    this->m_camera_corners.push_back(*camera_corners);
                    this->m_object_corners.push_back(*objct_corners);
                }

                return;
            }

            void clear() {
                this->m_valid = false;
                this->m_camera_corners.clear();
                this->m_object_corners.clear();
            }
        };

        template <uint32_t _format,uint32_t _num>
        class CollectCalibrationCorners;

        template <uint32_t _format>
        class CollectCalibrationCorners<_format,1> : public saliency_sandbox::core::Node::
        template Input<CameraCorners,ObjectCorners,bool>::
        template Output<
                CameraCornersList, // detected corners
                ObjectCornersList,  // actual corners
                bool
        > , protected CollectCalibrationCornersImpl<_format> {
        public:
            CollectCalibrationCorners() {
                this->template input<0>()->name("camera");
                this->template input<1>()->name("object");
                this->template input<2>()->name("valid");

                this->template output<0>()->name("camera");
                this->template output<0>()->value(&this->m_camera_corners);
                this->template output<1>()->name("object");
                this->template output<1>()->value(&this->m_object_corners);
                this->template output<2>()->name("valid");
                this->template output<2>()->value(&this->m_valid);
            }

            void calc() override {
                this->collect(
                        this->template input<0>()->value(),
                        this->template input<1>()->value(),
                        this->template input<2>()->value(),
                        this->properties());
            }

            void reset() override {
                this->clear();
            }
        };

        template <uint32_t _format>
        class CollectCalibrationCorners<_format,2> : public saliency_sandbox::core::Node::
        template Input<CameraCorners,ObjectCorners,bool,CameraCorners,ObjectCorners,bool>::
        template Output<
                CameraCornersList, // detected corners
                ObjectCornersList,  // actual corners
                bool
        > , protected CollectCalibrationCornersImpl<_format> {
        public:
            CollectCalibrationCorners() {
                this->template input<0>()->name("camera");
                this->template input<1>()->name("object");
                this->template input<2>()->name("valid");

                this->template output<0>()->name("camera");
                this->template output<0>()->value(&this->m_camera_corners);
                this->template output<1>()->name("object");
                this->template output<1>()->value(&this->m_object_corners);
                this->template output<2>()->name("valid");
                this->template output<2>()->value(&this->m_valid);
            }

            void calc() override {
                this->collect(this->template input<0>()->value(),
                              this->template input<1>()->value(),
                              this->template input<2>()->value(),
                              this->properties());
                this->collect(this->template input<3>()->value(),
                              this->template input<4>()->value(),
                              this->template input<5>()->value(),
                              this->properties());
            }

            void reset() override {
                this->clear();
            }
        };
    }
}
#endif //EYETRIBE_COLLECTCALIBRATIONCORNERS_H
