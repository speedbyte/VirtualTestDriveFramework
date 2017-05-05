//
// Created by geislerd on 12.04.17.
//
#include <eyetribe/VideoReader.h>
#include <io/ImageShow.h>
#include <plot/GridLayout.h>
#include <io/ImageWriter.h>

#define FORMAT 1

saliency_sandbox::eyetribe::VideoReader<FORMAT> vr0(0);
saliency_sandbox::eyetribe::VideoReader<FORMAT> vr1(1);

saliency_sandbox::plot::GridLayout<3,2> gl;

saliency_sandbox::io::ImageShow is("GridLayout");
saliency_sandbox::io::ImageWriter iw("/tmp/triggertest");


int main(int argc, char** argv) {
    saliency_sandbox::core::Utils::setMainStackSize();

    gl.template input<0>()->connect(vr0.template output<0>());
    gl.template input<1>()->connect(vr0.template output<1>());
    gl.template input<2>()->connect(vr0.template output<2>());
    gl.template input<3>()->connect(vr1.template output<0>());
    gl.template input<4>()->connect(vr1.template output<1>());
    gl.template input<5>()->connect(vr1.template output<2>());

    is.template input<0>()->connect(gl.template output<0>());
    //iw.template input<0>()->connect(vr0.template output<0>());

    for(time_t t = 0; true; t++) {
        is.process(t);
        //iw.process(t);
    }
}