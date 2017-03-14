//
// Created by geislerd on 10.02.17.
//

#ifndef KITTIIMAGEREADER_H
#define KITTIIMAGEREADER_H

#include <io/ImageReader.h>
#include <boost/filesystem/path.hpp>
#include <utils/Image.h>
#include <kitti/TrackletReader.h>

namespace saliency_sandbox {
    namespace kitti {
        enum Camera {
            LEFT_GRAY = 0,
            RIGHT_GRAY = 1,
            LEFT_RGB = 2,
            RIGHT_RGB = 3
        };

        template<Camera _camera>
        class _DrawTracklet;

        template<Camera _camera>
        class ImageReader { };

        template<>
        class ImageReader<LEFT_GRAY> : public saliency_sandbox::io::_ImageReader<1242,375,saliency_sandbox::utils::_IntensityImage> {
        public:

            typedef _DrawTracklet<LEFT_GRAY> DrawTracklet;

            ImageReader(boost::filesystem::path path) : saliency_sandbox::io::_ImageReader<1242,375,saliency_sandbox::utils::_IntensityImage>(path / "image_00" / "data") {
                this->template output<0>()->name("left_gray");
            };
        };

        template<>
        class ImageReader<RIGHT_GRAY> : public saliency_sandbox::io::_ImageReader<1242,375,saliency_sandbox::utils::_IntensityImage> {
        public:

            typedef _DrawTracklet<RIGHT_GRAY> DrawTracklet;

            ImageReader(boost::filesystem::path path) : saliency_sandbox::io::_ImageReader<1242,375,saliency_sandbox::utils::_IntensityImage>(path / "image_01" / "data") {
                this->template output<0>()->name("right_gray");
            };
        };

        template<>
        class ImageReader<LEFT_RGB> : public saliency_sandbox::io::_ImageReader<1242,375,saliency_sandbox::utils::_RGBImage> {
        public:

            typedef _DrawTracklet<LEFT_RGB> DrawTracklet;

            ImageReader(boost::filesystem::path path) : saliency_sandbox::io::_ImageReader<1242,375,saliency_sandbox::utils::_RGBImage>(path / "image_02" / "data") {
                this->template output<0>()->name("left_rgb");
            };
        };

        template<>
        class ImageReader<RIGHT_RGB> : public saliency_sandbox::io::_ImageReader<1242,375,saliency_sandbox::utils::_RGBImage> {
        public:

            typedef _DrawTracklet<RIGHT_RGB> DrawTracklet;

            ImageReader(boost::filesystem::path path) : saliency_sandbox::io::_ImageReader<1242,375,saliency_sandbox::utils::_RGBImage>(path / "image_03" / "data") {
                this->template output<0>()->name("right_rgb");
            };
        };

        typedef ImageReader<LEFT_GRAY> LeftGrayImageReader;
        typedef ImageReader<RIGHT_GRAY> RightGrayImageReader;
        typedef ImageReader<LEFT_RGB> LeftRGBImageReader;
        typedef ImageReader<RIGHT_RGB> RightRGBImageReader;

        template<Camera _camera>
        class _DrawTracklet : public saliency_sandbox::core::Node::template Input<typename ImageReader<_camera>::Image,TrackletList,Calibration>::template Output<saliency_sandbox::utils::_RGBImage<1242,375>> {
        public:
            typedef typename ImageReader<_camera>::Image input_image_t;
        private:
            saliency_sandbox::utils::_RGBImage<1242,375> m_data;

            void convert(input_image_t& img) {
                switch (_camera) {
                    case LEFT_GRAY:
                    case RIGHT_GRAY:
                        cv::cvtColor(img,this->m_data,CV_GRAY2RGB);
                        break;
                    case LEFT_RGB:
                    case RIGHT_RGB:
                        this->m_data.mat(img);
                        break;
                    default:
                        sserr << "unknown camera type: " << _camera << ssthrow;
                }
            }

            bool inImage(cv::Point* points, size_t num) {
                bool ret;

                ret = true;
                for(int i = 0; i < num && ret; i++) {
                    ret &= (points[i].x > 0 && points[i].y > 0) && (points[i].x < 1242 && points[i].y < 375);
                }

                return ret;
            }

            void drawTracklet(Tracklet& tracklet, Calibration& calibration) {
                cv::Vec3f faces3D[16];
                cv::Vec2f faces2D[16];
                cv::Point pf[4];
                cv::Point center(0,0);
                size_t num = 0;
                std::stringstream saliency_ss;

                tracklet.boundingbox(faces3D);
                calibration.veloToCam(faces3D,faces2D,16,_camera);

                for(int i = 0; i < 16; i+=4) {
                    pf[0] = cv::Point2f(faces2D[i+0]);
                    pf[1] = cv::Point2f(faces2D[i+1]);
                    pf[2] = cv::Point2f(faces2D[i+2]);
                    pf[3] = cv::Point2f(faces2D[i+3]);

                    if(this->inImage(pf,4)) {
                        cv::line(this->m_data, pf[0], pf[1], cv::Scalar(255, 0, 0));
                        cv::line(this->m_data, pf[1], pf[2], cv::Scalar(0, 255, 0));
                        cv::line(this->m_data, pf[2], pf[3], cv::Scalar(0, 0, 255));
                        cv::line(this->m_data, pf[3], pf[0], cv::Scalar(255, 0, 255));

                        center += pf[0] + pf[1] + pf[2] + pf[3];
                        num += 4;
                    }

                }

                if(num > 0) {
                    center.x /= num;
                    center.y /= num;
                }


                cv::putText(this->m_data,tracklet.label(),center,CV_FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(255,255,255), 1, CV_AA);

                if(tracklet.properties()->template has<float>("saliency")) {
                    saliency_ss << "s: " << std::setprecision(2) << tracklet.properties()->template get<float>("saliency");
                    cv::putText(this->m_data,saliency_ss.str(),cv::Point(center.x,center.y+14),CV_FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(255,255,255), 1, CV_AA);
                }

            }

            void drawTracklets(TrackletList& tracklets, Calibration& calibration) {
                for(int i = 0; i < tracklets.size(); i++)
                    this->drawTracklet(*tracklets[i],calibration);
            }

        public:

            _DrawTracklet() {
                this->template input<0>()->name("image");
                this->template input<1>()->name("tracklet");
                this->template input<2>()->name("calibration");

                this->template output<0>()->name("image");
                this->template output<0>()->value(&(this->m_data));
            }

            void calc() override {
                this->convert(*this->template input<0>()->value());
                this->drawTracklets(*this->template input<1>()->value(),*this->template input<2>()->value());

                this->template output<0>()->value(&(this->m_data));
            }

            void reset() override {

            }
        };

        template<Camera _camera>
        class TrackletSaliency : public saliency_sandbox::core::Node::template Input<
                TrackletList,
                saliency_sandbox::utils::_HeatmapImage<
                        saliency_sandbox::kitti::ImageReader<_camera>::Image::WIDTH,
                        saliency_sandbox::kitti::ImageReader<_camera>::Image::HEIGHT>,
                Calibration>::template Output<TrackletList> {
        public:
            TrackletSaliency() {
                this->template input<0>()->name("Tracklet");
                this->template input<1>()->name("Saliency Map");
                this->template input<2>()->name("Calibration");

                this->template output<0>()->name("Tracklet");
            }

            void calc() override {
                TrackletList* tl;
                Tracklet* t;
                Calibration* c;
                cv::Vec3f faces3D[16];
                cv::Vec2f faces2D[16];
                cv::Mat1f mat, smat;
                cv::Point2f min_b, max_b;
                cv::Rect bb;
                float sal;

                tl = this->template input<0>()->value();
                mat = this->template input<1>()->value()->mat();
                c = this->template input<2>()->value();

                for(int i = 0; i < tl->size(); i++) {
                    t = (*tl)[i];

                    t->boundingbox(faces3D);
                    c->veloToCam(faces3D,faces2D,16,_camera);

                    // find bounding box
                    min_b = cv::Point2f(faces2D[0]);
                    max_b = cv::Point2f(faces2D[0]);
                    for(int j = 1; j < 16; j++) {
                        min_b.x = MIN(min_b.x,faces2D[j].val[0]);
                        max_b.x = MAX(max_b.x,faces2D[j].val[0]);
                        min_b.y = MIN(min_b.y,faces2D[j].val[1]);
                        max_b.y = MAX(max_b.y,faces2D[j].val[1]);
                    }
                    min_b.x = MAX(min_b.x,0);
                    min_b.y = MAX(min_b.y,0);
                    max_b.x = MAX(max_b.x,0);
                    max_b.y = MAX(max_b.y,0);
                    min_b.x = MIN(min_b.x,mat.cols-1);
                    min_b.y = MIN(min_b.y,mat.rows-1);
                    max_b.x = MIN(max_b.x,mat.cols-1);
                    max_b.y = MIN(max_b.y,mat.rows-1);
                    bb = cv::Rect(min_b,max_b);

                    if(bb.area() <= 0)
                        continue;
                    smat = mat(bb);

                    sal = (float)cv::mean(smat).val[0];

                    t->properties()->template set<float>("saliency",sal);
                }

                this->template output<0>()->value(this->template input<0>()->value());
            }

            void reset() override {

            }
        };
    }
}
#endif //KITTIIMAGEREADER_H
