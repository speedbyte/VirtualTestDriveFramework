//
// Created by geislerd on 12.04.17.
//
#include <eyetribe/VideoReader.h>
#include <io/ImageShow.h>
#include <eyetribe/CameraCalibration.h>
#include <eyetribe/DrawCalibrationCorners.h>
#include <eyetribe/Undistortion.h>

#define FORMAT 2

saliency_sandbox::eyetribe::VideoReader<FORMAT> vr0(0);
saliency_sandbox::eyetribe::VideoReader<FORMAT> vr1(1);

saliency_sandbox::eyetribe::CameraCalibration<FORMAT> cc0;
saliency_sandbox::eyetribe::CameraCalibration<FORMAT> cc1;

saliency_sandbox::eyetribe::CameraUndistortion<FORMAT> cu0;
saliency_sandbox::eyetribe::CameraUndistortion<FORMAT> cu1;

saliency_sandbox::eyetribe::PointsUndistortion pu0;
saliency_sandbox::eyetribe::PointsUndistortion pu1;

saliency_sandbox::eyetribe::DrawCalibrationCorners<FORMAT> dc_raw_0;
saliency_sandbox::eyetribe::DrawCalibrationCorners<FORMAT> dc_raw_1;

saliency_sandbox::eyetribe::DrawCalibrationCorners<FORMAT> dc_und_0;
saliency_sandbox::eyetribe::DrawCalibrationCorners<FORMAT> dc_und_1;

saliency_sandbox::io::ImageShow is_raw_0("EyeTribe 0 RAW");
saliency_sandbox::io::ImageShow is_raw_1("EyeTribe 1 RAW");

saliency_sandbox::io::ImageShow is_und_0("EyeTribe 0 UNDISTORTED");
saliency_sandbox::io::ImageShow is_und_1("EyeTribe 1 UNDISTORTED");


int main(int argc, char** argv) {
    saliency_sandbox::core::Utils::setMainStackSize();

    cc0.template input<0>()->connect(vr0.template output<0>());
    //dc_und_0.template input<0>()->connect(vr0.template output<0>());
    //dc_und_0.template input<1>()->connect(cc0.template output<0>());
    //dc_und_0.template input<2>()->connect(cc0.template output<1>());
    cu0.template input<0>()->connect(vr0.template output<0>());
    cu0.template input<1>()->connect(cc0.template output<0>());
    cu0.template input<2>()->connect(cc0.template output<1>());

    pu0.template input<0>()->connect(cc0.template output<4>());
    pu0.template input<1>()->connect(cc0.template output<0>());
    pu0.template input<2>()->connect(cc0.template output<1>());

    dc_und_0.template input<0>()->connect(cu0.template output<0>());
    dc_und_0.template input<1>()->connect(pu0.template output<0>());
    dc_und_0.template input<2>()->connect(cc0.template output<5>());
    is_und_0.template input<0>()->connect(dc_und_0.template output<0>());

    dc_raw_0.template input<0>()->connect(vr0.template output<0>());
    dc_raw_0.template input<1>()->connect(cc0.template output<4>());
    dc_raw_0.template input<2>()->connect(cc0.template output<5>());
    is_raw_0.template input<0>()->connect(dc_raw_0.template output<0>());

    cc1.template input<0>()->connect(vr1.template output<0>());
    //dc_und_1.template input<0>()->connect(vr1.template output<0>());
    //dc_und_1.template input<1>()->connect(cc1.template output<0>());
    //dc_und_1.template input<2>()->connect(cc1.template output<1>());
    cu1.template input<0>()->connect(vr1.template output<0>());
    cu1.template input<1>()->connect(cc1.template output<0>());
    cu1.template input<2>()->connect(cc1.template output<1>());

    pu1.template input<0>()->connect(cc1.template output<4>());
    pu1.template input<1>()->connect(cc1.template output<0>());
    pu1.template input<2>()->connect(cc1.template output<1>());

    dc_und_1.template input<0>()->connect(cu1.template output<0>());
    dc_und_1.template input<1>()->connect(pu1.template output<0>());
    dc_und_1.template input<2>()->connect(cc1.template output<5>());
    is_und_1.template input<0>()->connect(dc_und_1.template output<0>());

    dc_raw_1.template input<0>()->connect(vr1.template output<0>());
    dc_raw_1.template input<1>()->connect(cc1.template output<4>());
    dc_raw_1.template input<2>()->connect(cc1.template output<5>());
    is_raw_1.template input<0>()->connect(dc_raw_1.template output<0>());

    for(time_t t = 0; true; t++) {
        is_und_0.process(t);
        is_raw_0.process(t);
        is_und_1.process(t);
        is_raw_1.process(t);
    }
}