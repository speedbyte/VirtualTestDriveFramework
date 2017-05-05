//
// Created by geislerd on 12.04.17.
//
#include <eyetribe/VideoReader.h>
#include <io/ImageShow.h>
#include <eyetribe/CameraCalibration.h>
#include <eyetribe/DrawCalibrationCorners.h>
#include <eyetribe/Undistortion.h>
#include <eyetribe/StereoCalibration.h>
#include <eyetribe/DrawStereoCalibration.h>
#include <eyetribe/StereoRectification.h>
#include <eyetribe/GlintDetector.h>
#include <eyetribe/StereoGlintMatcher.h>
#include <eyetribe/EyeballEstimation.h>
#include <eyetribe/StereoGlintFilter.h>
#include <eyetribe/DrawEyeball.h>
#include <eyetribe/PupilDetection.h>
#include <eyetribe/DrawGaze.h>
#include <plot/GridLayout.h>
#include <plot/Plot.h>
#include <io/VideoWriter.h>

#define FORMAT 1

saliency_sandbox::eyetribe::VideoReader<FORMAT> vr0(0);
saliency_sandbox::eyetribe::VideoReader<FORMAT> vr1(1);

saliency_sandbox::eyetribe::DetectCalibrationCorners<FORMAT> dcc0;
saliency_sandbox::eyetribe::DetectCalibrationCorners<FORMAT> dcc1;

saliency_sandbox::eyetribe::CollectCalibrationCorners<FORMAT,2> ccc;
saliency_sandbox::eyetribe::CameraCalibration<FORMAT> cc;

saliency_sandbox::eyetribe::DrawCalibrationCorners<FORMAT> drcc0;
saliency_sandbox::eyetribe::DrawCalibrationCorners<FORMAT> drcc1;

saliency_sandbox::eyetribe::StereoCalibration<FORMAT> sc;
saliency_sandbox::eyetribe::StereoRectification<FORMAT> sr;

saliency_sandbox::eyetribe::DrawStereoCalibration dsc;

saliency_sandbox::io::ImageShow is0("EyeTribe 0");
saliency_sandbox::io::ImageShow is1("EyeTribe 1");


saliency_sandbox::eyetribe::GlintDetector<FORMAT> gd0;
saliency_sandbox::eyetribe::GlintDetector<FORMAT> gd1;

saliency_sandbox::eyetribe::StereoGlintMatcher sgm;

saliency_sandbox::eyetribe::StereoGlintFilter sgf;

saliency_sandbox::eyetribe::EyeballEstimation ee;

saliency_sandbox::eyetribe::DrawEyeball<FORMAT> de;

saliency_sandbox::eyetribe::PupilDetection<FORMAT> pd0;
saliency_sandbox::eyetribe::PupilDetection<FORMAT> pd1;

saliency_sandbox::eyetribe::DrawGaze<FORMAT> dg;

saliency_sandbox::plot::GridLayout<4,2> gl;
saliency_sandbox::io::ImageShow is("GridLayout");
saliency_sandbox::gaze::Gaze::Selector<saliency_sandbox::gaze::Gaze::ORIGIN_X> lgsx;
saliency_sandbox::gaze::Gaze::Selector<saliency_sandbox::gaze::Gaze::ORIGIN_Y> lgsy;
saliency_sandbox::gaze::Gaze::Selector<saliency_sandbox::gaze::Gaze::ORIGIN_Z> lgsz;
saliency_sandbox::gaze::Gaze::Selector<saliency_sandbox::gaze::Gaze::ANGLE_INCLINATION> lgsi;
saliency_sandbox::gaze::Gaze::Selector<saliency_sandbox::gaze::Gaze::ANGLE_AZIMUTH> lgsa;
saliency_sandbox::gaze::Gaze::Selector<saliency_sandbox::gaze::Gaze::ORIGIN_X> rgsx;
saliency_sandbox::gaze::Gaze::Selector<saliency_sandbox::gaze::Gaze::ORIGIN_Y> rgsy;
saliency_sandbox::gaze::Gaze::Selector<saliency_sandbox::gaze::Gaze::ORIGIN_Z> rgsz;
saliency_sandbox::gaze::Gaze::Selector<saliency_sandbox::gaze::Gaze::ANGLE_INCLINATION> rgsi;
saliency_sandbox::gaze::Gaze::Selector<saliency_sandbox::gaze::Gaze::ANGLE_AZIMUTH> rgsa;

saliency_sandbox::plot::Plot pcart("left x","left y","left z","right x","right y","right z");
saliency_sandbox::plot::Plot pangle("left inclination","right azimuth","right inclination","right azimuth");

saliency_sandbox::io::VideoWriter vw("/home/geislerd/enkelejda_remote_tracking_eyetribe_2.avi");


int main(int argc, char** argv) {
    saliency_sandbox::core::Utils::setMainStackSize();

    dcc0.template input<0>()->connect(vr0.template output<0>());
    dcc1.template input<0>()->connect(vr1.template output<0>());

    ccc.template input<0>()->connect(dcc0.template output<0>());
    ccc.template input<1>()->connect(dcc0.template output<1>());
    ccc.template input<2>()->connect(dcc0.template output<2>());
    ccc.template input<3>()->connect(dcc1.template output<0>());
    ccc.template input<4>()->connect(dcc1.template output<1>());
    ccc.template input<5>()->connect(dcc1.template output<2>());

    cc.template input<0>()->connect(ccc.template output<0>());
    cc.template input<1>()->connect(ccc.template output<1>());
    cc.template input<2>()->connect(ccc.template output<2>());

    sc.template input<0>()->connect(dcc0.template output<0>());
    sc.template input<1>()->connect(dcc1.template output<0>());
    sc.template input<2>()->connect(cc.template output<0>());
    sc.template input<3>()->connect(cc.template output<1>());
    sc.template input<4>()->connect(cc.template output<0>());
    sc.template input<5>()->connect(cc.template output<1>());
    sc.template input<6>()->connect(cc.template output<2>());
    sc.template input<7>()->connect(cc.template output<2>());

    sr.template input<0>()->connect(cc.template output<0>());
    sr.template input<1>()->connect(cc.template output<1>());
    sr.template input<2>()->connect(cc.template output<0>());
    sr.template input<3>()->connect(cc.template output<1>());
    sr.template input<4>()->connect(sc.template output<0>());
    sr.template input<5>()->connect(sc.template output<1>());
    sr.template input<6>()->connect(sc.template output<4>());

    drcc0.template input<0>()->connect(vr0.template output<0>());
    drcc0.template input<1>()->connect(dcc0.template output<0>());
    drcc0.template input<2>()->connect(dcc0.template output<2>());
    drcc1.template input<0>()->connect(vr1.template output<0>());
    drcc1.template input<1>()->connect(dcc1.template output<0>());
    drcc1.template input<2>()->connect(dcc1.template output<2>());

    is0.template input<0>()->connect(drcc0.template output<0>());
    is1.template input<0>()->connect(drcc1.template output<0>());

    dsc.template input<0>()->connect(sc.template output<0>());
    dsc.template input<1>()->connect(sc.template output<1>());

    gd0.template input<0>()->connect(vr0.template output<0>());
    gd1.template input<0>()->connect(vr1.template output<0>());

    sgm.template input<0>()->connect(gd0.template output<0>());
    sgm.template input<1>()->connect(gd0.template output<1>());
    sgm.template input<2>()->connect(gd1.template output<0>());
    sgm.template input<3>()->connect(gd1.template output<1>());
    sgm.template input<4>()->connect(sr.template output<2>());
    sgm.template input<5>()->connect(sr.template output<3>());

    sgf.template input<0>()->connect(sgm.template output<0>());
    sgf.template input<1>()->connect(sgm.template output<1>());

    ee.template input<0>()->connect(sgf.template output<0>());
    ee.template input<1>()->connect(sgf.template output<1>());

    de.template input<0>()->connect(vr0.template output<0>());
    de.template input<1>()->connect(vr1.template output<0>());
    de.template input<2>()->connect(sr.template output<2>());
    de.template input<3>()->connect(sr.template output<3>());
    de.template input<4>()->connect(ee.template output<0>());
    de.template input<5>()->connect(ee.template output<1>());

    pd0.template input<0>()->connect(vr0.template output<0>());
    pd0.template input<1>()->connect(vr1.template output<0>());
    pd0.template input<2>()->connect(sr.template output<2>());
    pd0.template input<3>()->connect(sr.template output<3>());
    pd0.template input<4>()->connect(ee.template output<0>());

    pd1.template input<0>()->connect(vr0.template output<0>());
    pd1.template input<1>()->connect(vr1.template output<0>());
    pd1.template input<2>()->connect(sr.template output<2>());
    pd1.template input<3>()->connect(sr.template output<3>());
    pd1.template input<4>()->connect(ee.template output<1>());

    //dg.template input<0>()->connect(de.template output<0>());
    //dg.template input<1>()->connect(de.template output<1>());
    dg.template input<0>()->connect(vr0.template output<0>());
    dg.template input<1>()->connect(vr1.template output<0>());
    dg.template input<2>()->connect(sr.template output<2>());
    dg.template input<3>()->connect(sr.template output<3>());
    dg.template input<4>()->connect(pd0.template output<0>());
    dg.template input<5>()->connect(pd1.template output<0>());

    lgsx.template input<0>()->connect(pd0.template output<0>());
    lgsy.template input<0>()->connect(pd0.template output<0>());
    lgsz.template input<0>()->connect(pd0.template output<0>());
    lgsi.template input<0>()->connect(pd0.template output<0>());
    lgsa.template input<0>()->connect(pd0.template output<0>());
    rgsx.template input<0>()->connect(pd1.template output<0>());
    rgsy.template input<0>()->connect(pd1.template output<0>());
    rgsz.template input<0>()->connect(pd1.template output<0>());
    rgsi.template input<0>()->connect(pd1.template output<0>());
    rgsa.template input<0>()->connect(pd1.template output<0>());
    pcart.template input<0>()->connect(lgsx.template output<0>());
    pcart.template input<1>()->connect(lgsy.template output<0>());
    pcart.template input<2>()->connect(lgsz.template output<0>());
    pcart.template input<3>()->connect(rgsx.template output<0>());
    pcart.template input<4>()->connect(rgsy.template output<0>());
    pcart.template input<5>()->connect(rgsz.template output<0>());
    pangle.template input<0>()->connect(lgsi.template output<0>());
    pangle.template input<1>()->connect(lgsa.template output<0>());
    pangle.template input<2>()->connect(rgsi.template output<0>());
    pangle.template input<3>()->connect(rgsa.template output<0>());

    gl.template input<0>()->connect(drcc0.template output<0>());
    gl.template input<1>()->connect(drcc1.template output<0>());
    gl.template input<2>()->connect(pcart.template output<0>());
    gl.template input<3>()->connect(pangle.template output<0>());
    gl.template input<4>()->connect(de.template output<0>());
    gl.template input<5>()->connect(de.template output<1>());
    gl.template input<6>()->connect(dg.template output<0>());
    gl.template input<7>()->connect(dg.template output<1>());

    is.template input<0>()->connect(gl.template output<0>());
    vw.template input<0>()->connect(gl.template output<0>());

    for(time_t t = 0; true; t++) {
        dcc0.process(t);
        dcc0.process(t);
        //is.process(t);
        //dg.process(t);
        //is0.process(t);
        //is1.process(t);
        //vw.process(t);
    }
}