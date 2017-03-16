//
// Created by geislerd on 16.03.17.
//

#include <kitti/TrackletSaliency.h>

namespace saliency_sandbox {
    namespace kitti {

        template<Camera _camera>
        TrackletSaliency<_camera>::TrackletSaliency() {
            this->template input<0>()->name("Tracklet");
            this->template input<1>()->name("Saliency Map");
            this->template input<2>()->name("Calibration");

            this->template output<0>()->name("Tracklet");
        }

        template<Camera _camera>
        void TrackletSaliency<_camera>::calc() {
            TrackletList *tl;
            Tracklet *t;
            Calibration *c;
            cv::Vec3f faces3D[16];
            cv::Vec2f faces2D[16];
            cv::Mat1f mat, smat;
            cv::Point2f min_b, max_b;
            cv::Rect bb;
            float sal;

            tl = this->template input<0>()->value();
            mat = this->template input<1>()->value()->mat();
            c = this->template input<2>()->value();

            for (int i = 0; i < tl->size(); i++) {
                t = (*tl)[i];

                t->boundingbox(faces3D);
                c->veloToCam(faces3D, faces2D, 16, _camera);

                // find bounding box
                min_b = cv::Point2f(faces2D[0]);
                max_b = cv::Point2f(faces2D[0]);
                for (int j = 1; j < 16; j++) {
                    min_b.x = MIN(min_b.x, faces2D[j].val[0]);
                    max_b.x = MAX(max_b.x, faces2D[j].val[0]);
                    min_b.y = MIN(min_b.y, faces2D[j].val[1]);
                    max_b.y = MAX(max_b.y, faces2D[j].val[1]);
                }
                min_b.x = MAX(min_b.x, 0);
                min_b.y = MAX(min_b.y, 0);
                max_b.x = MAX(max_b.x, 0);
                max_b.y = MAX(max_b.y, 0);
                min_b.x = MIN(min_b.x, mat.cols - 1);
                min_b.y = MIN(min_b.y, mat.rows - 1);
                max_b.x = MIN(max_b.x, mat.cols - 1);
                max_b.y = MIN(max_b.y, mat.rows - 1);
                bb = cv::Rect(min_b, max_b);

                if (bb.area() <= 0)
                    continue;
                smat = mat(bb);

                sal = (float) cv::mean(smat).val[0];

                t->properties()->template set<float>("saliency", sal);
            }

            this->template output<0>()->value(this->template input<0>()->value());
        }

        template<Camera _camera>
        void TrackletSaliency<_camera>::reset() { }

        template class TrackletSaliency<LEFT_GRAY>;
        template class TrackletSaliency<RIGHT_GRAY>;
        template class TrackletSaliency<LEFT_RGB>;
        template class TrackletSaliency<RIGHT_RGB>;
    }
}