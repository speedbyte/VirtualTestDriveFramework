//
// Created by geislerd on 14.02.17.
//

#include <plot/Plot.h>
#include <sys/stat.h>
#include <fcntl.h>

namespace saliency_sandbox {
    namespace plot {

        void _NodeDummy::calc() {
            this->output<0>()->value(&this->m_dummy);
        }

        void _NodeDummy::reset() {

        }

        Line::Line(std::string name,saliency_sandbox::core::IPort* port, Gnuplot* gp) : m_name(name) , m_port(port) , m_gp(gp) , m_min(FLT_MAX) , m_max(-FLT_MAX) {
            port->connect(this->m_dummy.template output<0>());
        }

        bool Line::active() {
            return this->m_port->dependency() != this->m_dummy.template output<0>();
        }

        void Line::calc(size_t numPoints) {
            time_t time;
            float value;

            if(!this->active())
                return;

            time = this->m_port->time();
            value = *(float*)this->m_port->data();
            if(isnanf(value))
                value = 0;
            this->m_min = fminf(this->m_min,value);
            this->m_max = fmaxf(this->m_max,value);

            this->m_data.push_back(std::make_pair<time_t&,float&>(time,value));
            while(this->m_data.size() > numPoints)
                this->m_data.erase(this->m_data.begin());
        }

        float Line::minY() {
            return this->m_min;
        }

        float Line::maxY() {
            return this->m_max;
        }

        float Line::top() {
            return this->m_data.end()->second;
        }

        void Line::plot(bool smooth) {
            std::string type;

            type = "lines";
            if(smooth && this->m_data.size() >= 3)
                type += " smooth sbezier";

            (*this->m_gp) << this->m_gp->file1d(this->m_data) << "with " << type << " title '" << this->m_name << "'";
        }

        Plot::Plot(   std::string sig0,
                      std::string sig1,
                      std::string sig2,
                      std::string sig3,
                      std::string sig4,
                      std::string sig5,
                      std::string sig6,
                      std::string sig7,
                      std::string sig8,
                      std::string sig9) : m_lines({
                      Line(sig0,this->template input<0>(),&(this->m_gp)),Line(sig1,this->template input<1>(),&(this->m_gp)),
                      Line(sig2,this->template input<2>(),&(this->m_gp)),Line(sig3,this->template input<3>(),&(this->m_gp)),
                      Line(sig4,this->template input<4>(),&(this->m_gp)),Line(sig5,this->template input<5>(),&(this->m_gp)),
                      Line(sig6,this->template input<6>(),&(this->m_gp)),Line(sig7,this->template input<7>(),&(this->m_gp)),
                      Line(sig8,this->template input<8>(),&(this->m_gp)),Line(sig9,this->template input<9>(),&(this->m_gp))})
        {
            this->m_tmp /= "/tmp";
            this->m_tmp /= "saliency_sandbox_2";
            boost::filesystem::create_directories(this->m_tmp);
            this->m_tmp /= saliency_sandbox::core::Utils::uniqueName() + ".png";
            if(boost::filesystem::exists(this->m_tmp))
                boost::filesystem::remove(this->m_tmp);
            mkfifo(this->m_tmp.c_str(),0666);
            this->m_fifo = open(this->m_tmp.c_str(), O_RDONLY | O_NONBLOCK | O_CREAT);
        }

        cv::Mat3b Plot::readFIFO() {
            cv::Mat3b mat;
            int n,m;
            ssize_t r;
            uchar buffer[1024*100];

            n = 0;
            m = 0;
            do {
                usleep(m*100);
                r = read(this->m_fifo, &(buffer[n]), sizeof(buffer) - n);
                if(r > 0)
                    n += r;
                else {
                    if (n > 0) {
                        mat = cv::imdecode(cv::Mat1b(1, n, buffer), cv::IMREAD_COLOR, &mat);
                    }
                    m++;
                }

            } while((n<=0 || mat.cols == 0 || mat.rows == 0) && m < 100);

            if(m >= 100)
                throw new std::exception(); //TODO: error handling

            return mat;
        }

        void Plot::calc() {
            bool sep;
            int width, height;
            float minX, maxX;
            float minY, maxY;
            float expY;
            float top;
            std::stringstream stop;


            for(int i = 0; i  < this->numInput(); i++)
                if(this->m_lines[i].active())
                    this->m_lines[i].calc(this->numPoints());

            width = this->properties()->template get<int>("width",RES_WIDTH(RESOLUTION));
            height = this->properties()->template get<int>("height",RES_HEIGHT(RESOLUTION));
            minX = this->minX();
            maxX = this->maxX();
            minY = this->minY();
            maxY = this->maxY();

            //expand y range
            expY = (maxY-minY)*0.2f;
            minY -= expY;
            maxY += expY;

            // set png as output
            this->m_gp << "set term png size " << width << "," << height << " enhanced background rgb 'black'"<< std::endl;
            this->m_gp << "set border lc rgb 'white'"<< std::endl;
            this->m_gp << "set key tc rgb 'white'"<< std::endl;

            this->m_gp << "set y2tics (";
            sep = false;
            for(int i = 0; i  < this->numInput(); i++)
                if(this->m_lines[i].active()) {
                    stop = std::stringstream();
                    stop << std::setprecision(4) << std::fixed;
                    // dont print "," on first line
                    if(sep)
                        this->m_gp << ",";
                    else
                        sep = true;
                    top = this->m_lines[i].top();
                    stop << top;
                    this->m_gp << "'" << stop.str() << "' " << top;
                }
            this->m_gp << ")" << std::endl;

            // pipe output to fifo
            this->m_gp << "set output '| cat >&2 > " << this->m_tmp.string() << "'" << std::endl;

            // set x range
            this->m_gp << "set xrange [" << minX << ":" << maxX << "]" << std::endl;

            // set y range
            if(this->minY()<this->maxY())
                this->m_gp << "set yrange [" << minY << ":" << maxY << "]" << std::endl;

            // show grid
            this->m_gp << "set grid" << std::endl;

            // x label
            this->m_gp << "set xlabel 'time' tc rgb 'white'" << std::endl;

            this->m_gp << "plot";
            sep = false;
            for(int i = 0; i  < this->numInput(); i++) {
                if(this->m_lines[i].active()) {
                    // dont print "," on first line
                    if(sep)
                        this->m_gp << ",";
                    else
                        sep = true;
                    this->m_lines[i].plot(this->smooth());
                }
            }
            this->m_gp << std::endl;
            this->m_gp << std::flush;

            // read plot from fifo
            this->m_data << this->readFIFO();

            this->template output<0>()->value(&(this->m_data));
        }

        void Plot::reset() {
        }

        bool Plot::smooth() {
            return this->properties()->get<bool>("smooth",false);
        }

        size_t Plot::numPoints() {
            const int defaultMaxPoints = 25;
            int maxPoints;

            maxPoints = this->properties()->get<int>("max_points",defaultMaxPoints);
            if(maxPoints > 0)
                return (size_t) maxPoints;
            else
                return defaultMaxPoints; //TODO: error handling - max points must be positive and greater than zero
        }

        float Plot::minX() {
            if(this->proctime() < this->numPoints())
                return 0;
            else
                return this->proctime() - this->numPoints() + 1;
        }

        float Plot::maxX() {
            if(this->proctime() < this->numPoints())
                return this->numPoints();
            else
                return this->proctime();
        }

        float Plot::minY() {
            float v;

            v = FLT_MAX;
            for(int i = 0; i  < this->numInput(); i++) {
                if (this->m_lines[i].active()) {
                    v = fminf(v,this->m_lines[i].minY());
                }
            }

            return v;
        }

        float Plot::maxY() {
            float v;

            v = -FLT_MAX;
            for(int i = 0; i  < this->numInput(); i++) {
                if (this->m_lines[i].active()) {
                    v = fmaxf(v,this->m_lines[i].maxY());
                }
            }

            return v;
        }
    }
}