//
// Created by geislerd on 13.02.17.
//

#ifndef PLOT_H
#define PLOT_H

#include <core/Node.h>
#include <gnuplot-iostream.h>
#include <plot/Figure.h>

namespace saliency_sandbox {
    namespace plot {

        class _NodeDummy : public saliency_sandbox::core::Node::template Input<>::template Output<float> {
        private:
            float m_dummy;
        public:
            void calc() override;

            void reset() override;
        };

        class Line {
        private:
            _NodeDummy m_dummy;
            Gnuplot* m_gp;
            std::string m_name;
            std::vector<std::pair<time_t,float>> m_data;
            saliency_sandbox::core::IPort* m_port;
            float m_min;
            float m_max;

        public:
            Line(std::string name,saliency_sandbox::core::IPort* port, Gnuplot* gp);

            bool active();

            void calc(size_t numPoints);

            float top();

            void plot(bool smooth);

            float minY();

            float maxY();
        };

        class Plot : public saliency_sandbox::core::Node::template Input<float,float,float,float,float,float,float,float,float,float>::template Output<Figure> {
        private:
            Gnuplot m_gp;
            Line m_lines[10];
            boost::filesystem::path m_tmp;
            Figure m_data;
            int m_fifo;

            cv::Mat3b readFIFO();

        public:
            Plot(   std::string sig0 = "",
                    std::string sig1 = "",
                    std::string sig2 = "",
                    std::string sig3 = "",
                    std::string sig4 = "",
                    std::string sig5 = "",
                    std::string sig6 = "",
                    std::string sig7 = "",
                    std::string sig8 = "",
                    std::string sig9 = "");

            void calc() override;

            void reset() override;

            bool smooth();

            size_t numPoints();

            float minY();

            float maxY();

            float minX();

            float maxX();
        };
    }
}
#endif //PLOT_H
