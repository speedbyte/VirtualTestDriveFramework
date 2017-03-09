//
// Created by geislerd on 25.02.17.
//

#ifndef BOOLEANMAPS_H
#define BOOLEANMAPS_H

#include <saliency/Saliency.h>

namespace saliency_sandbox {
    namespace saliency {
        namespace activation {

            template<uint32_t _width, uint32_t _height>
            class _BooleanMaps;

            template<uint32_t _width, uint32_t _height>
            void _bm_thresh_thread(_BooleanMaps<_width,_height>* i, cv::Mat1b in, int thresh, int fs, double c, bool erode, bool dilate) {
                cv::Mat1b t;

                cv::adaptiveThreshold(in,t,UCHAR_MAX,CV_ADAPTIVE_THRESH_MEAN_C,thresh,fs,c);
                i->add(t,erode,dilate);
            }

            template<uint32_t _width, uint32_t _height>
            class _BooleanMaps : public _Saliency<_width,_height,saliency_sandbox::utils::_IntensityImage<_width,_height>> {
            private:
                cv::Mat1b m_open;

            public:
                _BooleanMaps() {
                    this->reset();
                }

                int filterSize() {
                    const int x = this->properties()->template get<int>("filter_size",4);

                    return x;
                }

                int numThresholds() {
                    const int n = this->properties()->template get<int>("num_thresholds",4);

                    return n;
                }

                int numFilterPyr() {
                    const int n = this->properties()->template get<int>("num_filter_pyr",2);

                    return n;
                }

                int openingSize() {
                    const int x = this->properties()->template get<int>("opening_size",3);

                    return x;
                }

                float smoothSigma() {
                    const float s = this->properties()->template get<float>("smooth_sigma",2.0f);

                    return s;
                }

                void floodFillBorder(cv::Mat1b& map) {
                    const cv::Point seedPoint(0,0);

                    /*
                     *  draw border
                     */
                    for (cv::Point i(0, 0); i.x < _width; i.x++)
                        map(i) = 255;
                    for (cv::Point i(0, _height - 1); i.x < _width; i.x++)
                        map(i) = 255;
                    for (cv::Point i(0, 0); i.y < _height; i.y++)
                        map(i) = 255;
                    for (cv::Point i(_width - 1, 0); i.y < _height; i.y++)
                        map(i) = 255;

                    /*
                     *  delete segments with border contact
                     */
                    cv::floodFill(map, seedPoint, 0);

                    cv::addWeighted(this->map(),1.0f,map,1.0f/float(UCHAR_MAX),0.0f,this->map());
                }

                void add(cv::Mat1b map, bool erode, bool dilate) {
                    if(erode)
                        cv::erode(map,map,this->m_open);

                    if(dilate)
                        cv::dilate(map,map,this->m_open);
                    this->floodFillBorder(map);

                }

                void calc() override {
                    cv::Mat1b in, t;
                    double hmin, hmax, r, c;
                    int fs;
                    std::vector<std::future<void>> calls;
                    float smooth = this->smoothSigma();

                    this->map(0.0f);

                    in = this->input()->value()->mat();

                    cv::minMaxLoc(in,&hmin,&hmax);
                    r = (hmax-hmin)/(4.0*this->numThresholds());

                    for(int i = 0; i < this->numThresholds(); i++)
                        for(int j = 0; j < this->numFilterPyr(); j++) {
                            fs = this->filterSize()*(j+1)*2+1;
                            c = (i-this->numThresholds()/2)*r;

                            /*
                            calls.push_back(std::async(std::launch::async,_bm_thresh_thread<_width,_height>,this,in,CV_THRESH_BINARY,fs,c,true,true));
                            calls.push_back(std::async(std::launch::async,_bm_thresh_thread<_width,_height>,this,in,CV_THRESH_BINARY_INV,fs,c,true,true));
                             */

                            cv::adaptiveThreshold(in,t,UCHAR_MAX,CV_ADAPTIVE_THRESH_MEAN_C,CV_THRESH_BINARY,fs,c);
                            this->add(t,true,true);

                            cv::adaptiveThreshold(in,t,UCHAR_MAX,CV_ADAPTIVE_THRESH_MEAN_C,CV_THRESH_BINARY_INV,fs,c);
                            this->add(t,true,true);
                        }

                    for(int i = 0; i < calls.size(); i++)
                        calls[i].get();

                    // blur
                    if(smooth > 0.0f) {
                        fs = this->filterSize();

                        cv::GaussianBlur(this->map(), this->map(), cv::Size(fs*2+1,fs*2+1), smooth);
                    }
                }

                void reset() override {
                    const int openRadi = this->openingSize();
                    cv::Size openSize = cv::Size(openRadi*2+1,openRadi*2+1);
                    cv::Point openCenter = cv::Point(openRadi,openRadi);

                    this->m_open = cv::Mat1b::ones(openSize);

                    for(cv::Point xy(0,0); xy.x < this->m_open.cols; xy.x++)
                        for(xy.y = 0; xy.y < this->m_open.rows; xy.y++)
                            if(cv::norm(xy-openCenter)>openRadi)
                                this->m_open(xy) = 0;
                }
            };

            typedef _BooleanMaps<RESOLUTION> BooleanMaps;
        }
    }
}
#endif //BOOLEANMAPS_H
