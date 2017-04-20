//
// Created by geislerd on 14.04.17.
//

#ifndef EYETRIBE_DRAWSTEREOCALIBRATION_H
#define EYETRIBE_DRAWSTEREOCALIBRATION_H
#include <eyetribe/StereoCalibration.h>

#define EYETRIBE_DRAWSTEREOCALIBRATION_RES 1000
#define EYETRIBE_DRAWSTEREOCALIBRATION_NUM_LINES 21

namespace saliency_sandbox {
    namespace eyetribe {

        typedef saliency_sandbox::utils::_RGBImage<EYETRIBE_DRAWSTEREOCALIBRATION_RES,EYETRIBE_DRAWSTEREOCALIBRATION_RES> StereoCalibrationImage;

        class DrawStereoCalibration : public saliency_sandbox::core::Node::
        template Input<
                CameraCameraRotation,      // rotation matrix from camera 0 to camera 1
                CameraCameraTranslation    // translation from camera 0 to camera 1
        >::template Output<StereoCalibrationImage> {
        private:
            StereoCalibrationImage m_image;
            cv::Matx34f m_P[4];
            cv::Matx23f m_P2D[4];
            cv::Rect m_R[4];
            cv::Point m_L[4][EYETRIBE_DRAWSTEREOCALIBRATION_NUM_LINES][2][2];
            cv::Scalar m_C[3][2];
            cv::Vec4f m_V[11][2];
        public:
            DrawStereoCalibration() {
                this->reset();
            }

            cv::Matx44f RT(uint32_t cam) {
                const CameraCameraRotation& CR = *this->template input<0>()->value();
                const CameraCameraTranslation& CT = *this->template input<1>()->value();

                switch (cam) {
                    case 0:
                        return cv::Matx44f::eye();
                    case 1:
                        return cv::Matx44f( CR(0),CR(1),CR(2),CT(0),
                                            CR(3),CR(4),CR(5),CT(1),
                                            CR(6),CR(7),CR(8),CT(2),
                                            0.0f ,0.0f ,0.0f ,1.0f );
                    default:
                        sserr << "cam index out of bounds: " << cam << ssthrow;
                }

            };

            void drawCam(uint32_t dim, uint32_t cam, cv::Scalar color) {
                cv::Matx44f RT = this->RT(cam);

                for(int i = 0; i < 8; i++) {
                    cv::line(this->m_image,
                             cv::Point2f(this->m_P2D[dim]*(this->m_P[dim]*(RT*this->m_V[i][0]))),
                                         cv::Point2f(this->m_P2D[dim]*(this->m_P[dim]*(RT*this->m_V[i][1]))),
                             color, 1);
                }
                cv::line(this->m_image,
                         cv::Point2f(this->m_P2D[dim]*(this->m_P[dim]*(RT*this->m_V[ 8][0]))),
                                     cv::Point2f(this->m_P2D[dim]*(this->m_P[dim]*(RT*this->m_V[ 8][1]))),
                         this->m_C[0][0], 1);
                cv::line(this->m_image,
                         cv::Point2f(this->m_P2D[dim]*(this->m_P[dim]*(RT*this->m_V[ 9][0]))),
                                     cv::Point2f(this->m_P2D[dim]*(this->m_P[dim]*(RT*this->m_V[ 9][1]))),
                         this->m_C[1][0], 1);
                cv::line(this->m_image,
                         cv::Point2f(this->m_P2D[dim]*(this->m_P[dim]*(RT*this->m_V[10][0]))),
                                     cv::Point2f(this->m_P2D[dim]*(this->m_P[dim]*(RT*this->m_V[10][1]))),
                         this->m_C[2][0], 1);
            }

            void drawAxis(uint32_t dim, std::string label0, std::string label1) {
                const int e = EYETRIBE_DRAWSTEREOCALIBRATION_NUM_LINES-1;
                const int c = e/2;

                // draw minor grid
                for(int i = 1; i < c; i++)
                    for(int j = 0; j < 2; j++)
                        cv::line(this->m_image, this->m_L[dim][i][j][0], this->m_L[dim][i][j][1], cv::Scalar::all(64.0));

                for(int i = c+1; i < e; i++)
                    for(int j = 0; j < 2; j++)
                        cv::line(this->m_image, this->m_L[dim][i][j][0], this->m_L[dim][i][j][1], cv::Scalar::all(64.0));

                cv::line(this->m_image, this->m_L[dim][c][0][0], this->m_L[dim][c][0][1], this->m_C[dim][0], 2); // draw major axis
                cv::line(this->m_image, this->m_L[dim][c][1][0], this->m_L[dim][c][1][1], this->m_C[dim][1], 2); //

                cv::line(this->m_image, this->m_L[dim][0][0][0], this->m_L[dim][0][0][1], cv::Scalar::all(255.0));  // draw border
                cv::line(this->m_image, this->m_L[dim][0][1][0], this->m_L[dim][0][1][1], cv::Scalar::all(255.0));  //
                cv::line(this->m_image, this->m_L[dim][e][0][0], this->m_L[dim][e][0][1], cv::Scalar::all(255.0));  //
                cv::line(this->m_image, this->m_L[dim][e][1][0], this->m_L[dim][e][1][1], cv::Scalar::all(255.0));  //

                cv::putText(this->m_image,label0,this->m_L[dim][c][0][1]-cv::Point(30,10),CV_FONT_HERSHEY_SIMPLEX,1.0f,cv::Scalar::all(255.0));
                cv::putText(this->m_image,label1,this->m_L[dim][c][1][1]-cv::Point(30,10),CV_FONT_HERSHEY_SIMPLEX,1.0f,cv::Scalar::all(255.0));
            }

            void draw(uint32_t dim, std::string label0, std::string label1) {
                this->drawAxis(dim,label0,label1);
                this->drawCam(dim,0,cv::Scalar(255.0,255.0,0.0));
                this->drawCam(dim,1,cv::Scalar(0.0,255.0,255.0));
            }

            void calc() override {
                this->m_image.clear();

                this->draw(0,"X","Y");
                this->draw(1,"Y","Z");
                this->draw(2,"Z","X");

                cv::namedWindow(std::string("DEBUG DSC"));
                cv::imshow("DEBUG DSC", this->m_image);
            }


            void reset() override {
                cv::Vec4f p4[2][2];
                cv::Vec3f p3[2][2];
                cv::Vec2f p2[2][2];
                cv::Scalar c[3];
                float step;

                this->m_R[0] = cv::Rect(0,0,EYETRIBE_DRAWSTEREOCALIBRATION_RES/2,EYETRIBE_DRAWSTEREOCALIBRATION_RES/2);
                this->m_R[1] = cv::Rect(EYETRIBE_DRAWSTEREOCALIBRATION_RES/2,0,EYETRIBE_DRAWSTEREOCALIBRATION_RES/2,EYETRIBE_DRAWSTEREOCALIBRATION_RES/2);
                this->m_R[2] = cv::Rect(0,EYETRIBE_DRAWSTEREOCALIBRATION_RES/2,EYETRIBE_DRAWSTEREOCALIBRATION_RES/2,EYETRIBE_DRAWSTEREOCALIBRATION_RES/2);

                // translate to viewport
                //
                // +-----+-----+
                // | X-Y | Y-Z |
                // +-----+-----+
                // | Z-X |     |
                // +-----+-----+
                this->m_P[0] = cv::Matx34f(
                        1.0f,0.0f,0.0f,EYETRIBE_DRAWSTEREOCALIBRATION_RES/4.0f+this->m_R[0].x,  // X-Y Plane
                        0.0,1.0f,0.0f,EYETRIBE_DRAWSTEREOCALIBRATION_RES/4.0f+this->m_R[0].y,  //
                        0.0,0.0f,0.0f,EYETRIBE_DRAWSTEREOCALIBRATION_RES/4.0f);                //
                this->m_P[1] = cv::Matx34f(
                        0.0,0.0f,0.0f,EYETRIBE_DRAWSTEREOCALIBRATION_RES/4.0f,                 // Y-Z Plane
                        0.0,1.0f,0.0f,EYETRIBE_DRAWSTEREOCALIBRATION_RES/4.0f+this->m_R[1].x,  //
                        0.0,0.0f,1.0f,EYETRIBE_DRAWSTEREOCALIBRATION_RES/4.0f+this->m_R[1].y); //
                this->m_P[2] = cv::Matx34f(
                        1.0f,0.0f,0.0f,EYETRIBE_DRAWSTEREOCALIBRATION_RES/4.0f+this->m_R[2].x,  // Z-X Plane
                        0.0f,0.0f,0.0f,EYETRIBE_DRAWSTEREOCALIBRATION_RES/4.0f,                 //
                        0.0f,0.0f,1.0f,EYETRIBE_DRAWSTEREOCALIBRATION_RES/4.0f+this->m_R[2].y); //

                // 3D to 2D
                this->m_P2D[0] = cv::Matx23f( // X-Y Plane
                        1.0f,0.0f,0.0f,
                        0.0f,1.0f,0.0f);
                this->m_P2D[1] = cv::Matx23f( // Y-Z Plane
                        0.0f,1.0f,0.0f,
                        0.0f,0.0f,1.0f);
                this->m_P2D[2] = cv::Matx23f( // Z-X Plane
                        1.0f,0.0f,0.0f,
                        0.0f,0.0f,1.0f);

                // colors
                c[0] = cv::Scalar(128.0,0.0,0.0); // X-Y (red)
                c[1] = cv::Scalar(0.0,128.0,0.0); // Y-Z (green)
                c[2] = cv::Scalar(0.0,0.0,128.0); // Z-X (blue)

                step = float(EYETRIBE_DRAWSTEREOCALIBRATION_RES/(2*(EYETRIBE_DRAWSTEREOCALIBRATION_NUM_LINES-1)));

                // prepare grid
                for(int dim = 0; dim < 3; dim++) { // x-y, y-z, z-x
                    for(int l = 0; l < EYETRIBE_DRAWSTEREOCALIBRATION_NUM_LINES; l++) {
                        p4[0][0] = cv::Vec4f(0.0f,0.0f,0.0f,1.0f);
                        p4[0][1] = cv::Vec4f(0.0f,0.0f,0.0f,1.0f);
                        p4[1][0] = cv::Vec4f(0.0f,0.0f,0.0f,1.0f);
                        p4[1][1] = cv::Vec4f(0.0f,0.0f,0.0f,1.0f);

                        p4[0][0].val[dim] = -float(EYETRIBE_DRAWSTEREOCALIBRATION_RES / 4.0f);
                        p4[0][1].val[dim] = +float(EYETRIBE_DRAWSTEREOCALIBRATION_RES / 4.0f);
                        p4[1][0].val[dim] = (l-(EYETRIBE_DRAWSTEREOCALIBRATION_NUM_LINES-1)/2) * step;
                        p4[1][1].val[dim] = (l-(EYETRIBE_DRAWSTEREOCALIBRATION_NUM_LINES-1)/2) * step;

                        p4[0][0].val[(dim+1) % 3] = (l-(EYETRIBE_DRAWSTEREOCALIBRATION_NUM_LINES-1)/2) * step;
                        p4[0][1].val[(dim+1) % 3] = (l-(EYETRIBE_DRAWSTEREOCALIBRATION_NUM_LINES-1)/2) * step;
                        p4[1][0].val[(dim+1) % 3] = -float(EYETRIBE_DRAWSTEREOCALIBRATION_RES / 4.0f);
                        p4[1][1].val[(dim+1) % 3] = +float(EYETRIBE_DRAWSTEREOCALIBRATION_RES / 4.0f);

                        // transform to viewport
                        p3[0][0] = this->m_P[dim] * p4[0][0];
                        p3[0][1] = this->m_P[dim] * p4[0][1];
                        p3[1][0] = this->m_P[dim] * p4[1][0];
                        p3[1][1] = this->m_P[dim] * p4[1][1];

                        // 3D to 2D
                        p2[0][0] = this->m_P2D[dim] * p3[0][0];
                        p2[0][1] = this->m_P2D[dim] * p3[0][1];
                        p2[1][0] = this->m_P2D[dim] * p3[1][0];
                        p2[1][1] = this->m_P2D[dim] * p3[1][1];

                        this->m_L[dim][l][0][0] = cv::Point2f(p2[0][0]);
                        this->m_L[dim][l][0][1] = cv::Point2f(p2[0][1]);
                        this->m_L[dim][l][1][0] = cv::Point2f(p2[1][0]);
                        this->m_L[dim][l][1][1] = cv::Point2f(p2[1][1]);

                        this->m_C[dim][0] = c[dim];
                        this->m_C[dim][1] = c[(dim+1)%3];
                    }
                }

                // prepare camera symbol
                this->m_V[0][0] = cv::Vec4f(-0.5f,-0.5f,1.0f,1.0f);
                this->m_V[0][1] = cv::Vec4f(+0.5f,-0.5f,1.0f,1.0f);
                this->m_V[1][0] = cv::Vec4f(+0.5f,-0.5f,1.0f,1.0f);
                this->m_V[1][1] = cv::Vec4f(+0.5f,+0.5f,1.0f,1.0f);
                this->m_V[2][0] = cv::Vec4f(+0.5f,+0.5f,1.0f,1.0f);
                this->m_V[2][1] = cv::Vec4f(-0.5f,+0.5f,1.0f,1.0f);
                this->m_V[3][0] = cv::Vec4f(-0.5f,+0.5f,1.0f,1.0f);
                this->m_V[3][1] = cv::Vec4f(-0.5f,-0.5f,1.0f,1.0f);

                this->m_V[4][0] = cv::Vec4f(-0.5f,-0.5f,1.0f,1.0f);
                this->m_V[4][1] = cv::Vec4f( 0.0f, 0.0f,0.0f,1.0f);
                this->m_V[5][0] = cv::Vec4f(+0.5f,-0.5f,1.0f,1.0f);
                this->m_V[5][1] = cv::Vec4f( 0.0f, 0.0f,0.0f,1.0f);
                this->m_V[6][0] = cv::Vec4f(+0.5f,+0.5f,1.0f,1.0f);
                this->m_V[6][1] = cv::Vec4f( 0.0f,0.0f,0.0f,1.0f);
                this->m_V[7][0] = cv::Vec4f(-0.5f,+0.5f,1.0f,1.0f);
                this->m_V[7][1] = cv::Vec4f( 0.0f,0.0f,0.0f,1.0f);

                this->m_V[8][0] = cv::Vec4f(1.0f,0.0f,0.0f,1.0f);
                this->m_V[8][1] = cv::Vec4f(0.0f,0.0f,0.0f,1.0f);
                this->m_V[9][0] = cv::Vec4f(0.0f,1.0f,0.0f,1.0f);
                this->m_V[9][1] = cv::Vec4f(0.0f,0.0f,0.0f,1.0f);
                this->m_V[10][0] = cv::Vec4f(0.0f,0.0f,1.0f,1.0f);
                this->m_V[10][1] = cv::Vec4f(0.0f,0.0f,0.0f,1.0f);

                for(int i = 0; i < 11; i++)
                    for(int j = 0; j < 2; j++) {
                        this->m_V[i][j][0] *= step;
                        this->m_V[i][j][1] *= step;
                        this->m_V[i][j][2] *= step;
                    }
            };
        };
    }
}

#endif //EYETRIBE_DRAWSTEREOCALIBRATION_H
