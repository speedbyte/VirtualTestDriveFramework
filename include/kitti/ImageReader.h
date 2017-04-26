//
// Created by geislerd on 10.02.17.
//

#ifndef KITTIIMAGEREADER_H
#define KITTIIMAGEREADER_H

#include <io/ConvertImage.h>
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
        class ImageReader { };

        template<>
        class ImageReader<LEFT_GRAY> : public saliency_sandbox::io::_ImageReader<1242,375,saliency_sandbox::utils::_IntensityImage> {
        public:
            ImageReader(boost::filesystem::path path);
        };

        template<>
        class ImageReader<RIGHT_GRAY> : public saliency_sandbox::io::_ImageReader<1242,375,saliency_sandbox::utils::_IntensityImage> {
        public:
            ImageReader(boost::filesystem::path path);
        };

        template<>
        class ImageReader<LEFT_RGB> : public saliency_sandbox::io::_ImageReader<1242,375,saliency_sandbox::utils::_RGBImage> {
        public:
            ImageReader(boost::filesystem::path path);
        };

        template<>
        class ImageReader<RIGHT_RGB> : public saliency_sandbox::io::_ImageReader<1242,375,saliency_sandbox::utils::_RGBImage> {
        public:
            ImageReader(boost::filesystem::path path);
        };

        typedef ImageReader<LEFT_GRAY> LeftGrayImageReader;
        typedef ImageReader<RIGHT_GRAY> RightGrayImageReader;
        typedef ImageReader<LEFT_RGB> LeftRGBImageReader;
        typedef ImageReader<RIGHT_RGB> RightRGBImageReader;
    }
}
#endif //KITTIIMAGEREADER_H
