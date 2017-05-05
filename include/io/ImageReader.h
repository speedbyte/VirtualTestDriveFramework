//
// Created by geislerd on 07.02.17.
//

#ifndef IMAGEREADER_H
#define IMAGEREADER_H

#include <boost/filesystem.hpp>

#include <config.h>
#include <core/Node.h>
#include <utils/Image.h>
#include <io/DirectoryScanner.h>

namespace saliency_sandbox {
    namespace io {

        template <uint32_t _width, uint32_t _height, template<uint32_t,uint32_t> typename _image_type>
        class _ImageReader : public core::Node::template Input<>::template Output<_image_type<_width,_height> > {
        public:
            typedef typename _image_type<_width,_height>::Mat Mat;
            typedef _image_type<_width,_height> Image;
        private:
            _image_type<_width,_height> m_data;
            DirectoryScanner m_dir;

        public:
            _ImageReader(boost::filesystem::path path) : m_dir(path) {
                //TODO: check path
                this->reset();

                this->output()->name("image");
            }

            virtual void reset() override {
                this->m_dir.reset();
            }

            virtual void calc() override {
                boost::filesystem::path image;
                Mat raw;

                this->m_dir.calc();

                if(!this->m_dir.eof()) {
                    image = *this->m_dir.template output<0>()->value();
                    raw = cv::imread(image.c_str(),cv::IMREAD_UNCHANGED);
                    if(raw.size[0]>0 && raw.size[1]>0) {
                        cv::resize(raw, this->m_data, this->m_data.size());
                    }
                    else
                        ; //TODO: error handling - video reader returns empty mat
                } else {
                    this->m_data.setTo(0);
                    this->eof(true);
                }

                this->output()->value(&(this->m_data));
            }
        };

        typedef _ImageReader<RESOLUTION,saliency_sandbox::utils::_RGBImage> RGBImageReader;
        typedef _ImageReader<RESOLUTION,saliency_sandbox::utils::_IntensityImage> IntensityImageReader;
    }
}

#endif //IMAGEREADER_H
