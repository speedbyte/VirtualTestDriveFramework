//
// Created by geislerd on 12.04.17.
//
#include <eyetribe/VideoReader.h>
#include <io/ImageShow.h>
#include <eyetribe/CameraCalibration.h>
#include <eyetribe/DrawCalibrationCorners.h>

#define FORMAT 2

saliency_sandbox::eyetribe::VideoReader<FORMAT> vr0(0);
saliency_sandbox::eyetribe::VideoReader<FORMAT> vr1(1);

saliency_sandbox::eyetribe::CameraCalibration<FORMAT> cc0;
saliency_sandbox::eyetribe::CameraCalibration<FORMAT> cc1;

saliency_sandbox::eyetribe::DrawCalibrationCorners<FORMAT> dc0;
saliency_sandbox::eyetribe::DrawCalibrationCorners<FORMAT> dc1;

saliency_sandbox::io::ImageShow is0("EyeTribe 0");
saliency_sandbox::io::ImageShow is1("EyeTribe 1");


int main(int argc, char** argv) {
    saliency_sandbox::core::Utils::setMainStackSize();

    cc0.template input<0>()->connect(vr0.template output<0>());
    cc1.template input<0>()->connect(vr1.template output<0>());


    dc0.template input<0>()->connect(vr0.template output<0>());
    dc0.template input<1>()->connect(cc0.template output<1>());
    dc0.template input<2>()->connect(cc0.template output<2>());
    dc1.template input<0>()->connect(vr1.template output<0>());
    dc1.template input<1>()->connect(cc1.template output<1>());
    dc1.template input<2>()->connect(cc1.template output<2>());

    is0.template input<0>()->connect(dc0.template output<0>());
    is1.template input<0>()->connect(dc1.template output<0>());

    while(true) {
        is0.process(-1);
        is1.process(-1);
    }
}