//
// Created by geislerd on 16.03.17.
//

#include <kitti/ImageReader.h>

namespace saliency_sandbox {
    namespace kitti {

        ImageReader<LEFT_GRAY>::ImageReader(boost::filesystem::path path) : saliency_sandbox::io::_ImageReader<1242,375,saliency_sandbox::utils::_IntensityImage>(path / "image_00" / "data") {
            this->template output<0>()->name("left_gray");
        }

        ImageReader<RIGHT_GRAY>::ImageReader(boost::filesystem::path path) : saliency_sandbox::io::_ImageReader<1242,375,saliency_sandbox::utils::_IntensityImage>(path / "image_01" / "data") {
            this->template output<0>()->name("right_gray");
        }

        ImageReader<LEFT_RGB>::ImageReader(boost::filesystem::path path) : saliency_sandbox::io::_ImageReader<1242,375,saliency_sandbox::utils::_RGBImage>(path / "image_02" / "data") {
            this->template output<0>()->name("left_rgb");
        }

        ImageReader<RIGHT_RGB>::ImageReader(boost::filesystem::path path) : saliency_sandbox::io::_ImageReader<1242,375,saliency_sandbox::utils::_RGBImage>(path / "image_03" / "data") {
            this->template output<0>()->name("right_rgb");
        }

        template class ImageReader<LEFT_GRAY>;
        template class ImageReader<RIGHT_GRAY>;
        template class ImageReader<LEFT_RGB>;
        template class ImageReader<RIGHT_RGB>;
    }
}