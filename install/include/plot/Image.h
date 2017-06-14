//
// Created by geislerd on 13.02.17.
//

#ifndef PLOT_IMAGE_H
#define PLOT_IMAGE_H

#include <core/Node.h>
#include <plot/Figure.h>

namespace saliency_sandbox {
    namespace plot {

        class Image : public saliency_sandbox::core::Node::template Input<saliency_sandbox::utils::IMatrix>::template Output<Figure> {
        private:
            Figure m_data;

        public:

            void calc() override {
                this->m_data << this->template input<0>()->value()->mat();

                this->template output<0>()->value(&(this->m_data));
            }

            void reset() override {

            }
        };
    }
}
#endif //PLOT_IMAGE_H
