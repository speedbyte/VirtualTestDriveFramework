//
// Created by geislerd on 14.02.17.
//

#ifndef GRIDLAYOUT_H
#define GRIDLAYOUT_H

#include <core/Node.h>
#include <plot/Figure.h>

namespace saliency_sandbox {
    namespace plot {

        template<uint32_t _num, typename... _types>
        class _GridLayoutInputRepeater : public _GridLayoutInputRepeater<_num-1,_types...,Figure> {
        private:
            static const int _index = sizeof...(_types);
            typedef _GridLayoutInputRepeater<_num-1,Figure,_types...> _base;

        public:
            _GridLayoutInputRepeater() {
                this->template input<_index>()->checkPortSize(false);
            }

            template<uint32_t _width, uint32_t _height, uint32_t _cols, uint32_t _rows>
            void subplot(Figure* out) {
                const uint32_t _row = _index / _cols;
                const uint32_t _col = _index - _row * _cols;
                cv::Rect rect;
                cv::Mat3b subfig;
                Figure* in;

                rect = cv::Rect(_col*_width,_row*_height,_width,_height);
                subfig = out->mat();
                subfig = subfig(rect);
                in = this->template input<_index>()->value();

                cv::resize(in->mat(),subfig,subfig.size());

                this->_base::template subplot<_width,_height,_cols,_rows>(out);
            }
        };

        template<typename... _types>
        class _GridLayoutInputRepeater<0,_types...> : public saliency_sandbox::core::Node::template Input<_types...>::template Output<Figure> {
        public:
            template<uint32_t _width, uint32_t _height, uint32_t _cols, uint32_t _rows>
            void subplot(Figure* out) { }
        };

        template<uint32_t _width, uint32_t _height, uint32_t _cols, uint32_t _rows>
        class _GridLayout : public _GridLayoutInputRepeater<_cols*_rows> {
        private:
            saliency_sandbox::utils::_RGBImage<_width*_cols,_height*_rows> m_data;
            Figure m_figure;
        public:
            _GridLayout() {
                this->m_figure << this->m_data;
                this->template output<0>()->value(&(this->m_figure));
            }

            void calc() override {
                this->template subplot<_width,_height,_cols,_rows>(&(this->m_figure));
                this->template output<0>()->value(&(this->m_figure));
            }

            void reset() override {

            }
        };

        template<uint32_t _cols, uint32_t _rows>
        class GridLayout : public _GridLayout<RESOLUTION,_cols,_rows> {
        public:
            GridLayout() {
            }
        };
    }
}

#endif //GRIDLAYOUT_H
