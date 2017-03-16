//
// Created by geislerd on 08.02.17.
//

#ifndef SALIENCY_H
#define SALIENCY_H

#include <cstdint>
#include <core/Node.h>
#include <utils/Image.h>

namespace saliency_sandbox {
    namespace saliency {

        template<uint32_t _width, uint32_t _height>
        class _SaliencyMap : public saliency_sandbox::utils::_HeatmapImage<_width, _height> {

        };

        template<uint32_t _width, uint32_t _height, typename... _input>
        class _Saliency : public core::Node::template Input<_input...>::template Output<_SaliencyMap<_width, _height>> {
        public:
            typedef _SaliencyMap<_width, _height> SaliencyMap;

        private:
            SaliencyMap m_map;

        protected:

            cv::Mat1f map() {
                return this->m_map.mat();
            }

            void map(cv::Mat1f map) {
                this->m_map.mat(map);
            }

            void map(float v) {
                cv::Mat1f m;

                m = this->map();
                for(cv::Point xy(0,0); xy.x < m.cols; xy.x++)
                    for(xy.y = 0; xy.y < m.rows; xy.y++)
                        m(xy) = v;
            }

            void normalize() {
                if (this->properties()->template get<bool>("normalize_output", true))
                    cv::normalize(this->map(), this->map(), 0, 1, cv::NORM_MINMAX);
            }

            void attenuate() {
                if (!this->properties()->template get<bool>("attenuate_border", true))
                    return;
                //TODO: Not implemented
            }

        public:
            _Saliency() {
                this->template output<0>()->name("saliency");
                this->template output<0>()->value(&(this->m_map));

                this->template input<0>()->name("feature");
            };
        };
    }
}
#endif //SALIENCY_H
