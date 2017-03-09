//
// Created by geislerd on 21.02.17.
//

#include <kitti/CalibrationReader.h>

namespace saliency_sandbox {
    namespace kitti {

        cv::Matx<float, 1, 2>& Calibration::Cam2Cam::S() {
            return this->m_S;
        }

        cv::Matx<float, 3, 3>& Calibration::Cam2Cam::K() {
            return this->m_K;
        }

        cv::Matx<float, 1, 5>& Calibration::Cam2Cam::D() {
            return this->m_D;
        };

        cv::Matx<float, 3, 3>& Calibration::Cam2Cam::R() {
            return this->m_R;
        };

        cv::Matx<float, 1, 3>& Calibration::Cam2Cam::T() {
            return this->m_T;
        };

        cv::Matx<float, 1, 2>& Calibration::Cam2Cam::S_rect() {
            return this->m_S_rect;
        };

        cv::Matx<float, 3, 3>& Calibration::Cam2Cam::R_rect() {
            return this->m_R_rect;
        };

        cv::Matx<float, 3, 4>& Calibration::Cam2Cam::P_rect() {
            return this->m_P_rect;
        };

        cv::Matx<float, 3, 3>& Calibration::Cam2Cam::R_rect_00() {
            return this->m_R_rect_00;
        };

        float& Calibration::Cam2Cam::cornerDist() {
            return this->m_corner_dist;
        }

        cv::Matx<float,4,4> Calibration::Cam2Cam::TR() {
            cv::Matx<float,4,4> T;
            cv::Matx<float,4,4> R;

            T = cv::Matx<float,4,4>::eye();
            T(0,3) = this->m_P_rect(0,3) / this->m_P_rect(0,0);
            R = cv::Matx<float,4,4>(
                    this->m_R_rect_00.val[0], this->m_R_rect_00.val[1], this->m_R_rect_00.val[2], 0,
                    this->m_R_rect_00.val[3], this->m_R_rect_00.val[4], this->m_R_rect_00.val[5], 0,
                    this->m_R_rect_00.val[6], this->m_R_rect_00.val[7], this->m_R_rect_00.val[8], 0,
                    0 , 0 , 0 , 1 );

            return T*R;
        };

        cv::Matx<float, 3, 3>& Calibration::Velo2Cam::R() {
            return this->m_R;
        };

        cv::Matx<float, 1, 3>& Calibration::Velo2Cam::T() {
            return this->m_T;
        };

        cv::Vec2f& Calibration::Velo2Cam::deltaF() {
            return this->m_delta_f;
        }

        cv::Vec2f& Calibration::Velo2Cam::deltaC() {
            return this->m_delta_c;
        }

        cv::Matx<float,4,4> Calibration::Velo2Cam::TR() {
            return cv::Matx<float,4,4>(
                    this->m_R.val[0],this->m_R.val[1],this->m_R.val[2],this->m_T.val[0],
                    this->m_R.val[3],this->m_R.val[4],this->m_R.val[5],this->m_T.val[1],
                    this->m_R.val[6],this->m_R.val[7],this->m_R.val[8],this->m_T.val[2],
                    0,0,0,1);
        };

        cv::Matx<float, 3, 3>& Calibration::OXTS2Velo::R() {
            return this->m_R;
        };

        cv::Matx<float, 3, 1>& Calibration::OXTS2Velo::T() {
            return this->m_T;
        };

        std::string Calibration::read(std::ifstream& is, size_t num, float* val) {
            std::string name;

            sserr << sscond(!(is >> name)) << "cannot read variable name from input stream. file may corrupt" << ssthrow;
            for(int i = 0; i < num; i++)
                sserr << sscond(!(is >> val[i])) << "cannot read float from input stream (variable: " << name << "). file may corrupt" << ssthrow;

            return name.substr(0,name.length()-1);
        }

        std::string Calibration::varName(std::string name, int num) {
            std::stringstream ss;

            ss << name << "_" << std::setw(2) << std::setfill('0') << num;

            return ss.str();
        }

        void Calibration::loadCamToCam(boost::filesystem::path path) {
            std::ifstream is(path.string());
            std::string name, head;
            float cornerDist;

            // check file path and stream state
            sserr << sspfile(path) << ssthrow;
            sserr << sscond(!is.is_open()) << "cannot open input file stream: " << path << ssthrow;

            // read head line
            std::getline(is,head);
            sserr << sscond(head.empty()) << "error while reading header in file: " << path << ssthrow;

            name = this->read(is,1,&cornerDist);
            sserr << ssequal(name , "corner_dist") << ssthrow;

            for(int i = 0; i < sizeof(this->m_camera)/sizeof(Cam2Cam); i++) {
                this->m_camera[i].cornerDist() = cornerDist;

                name = this->read(is, 2, this->m_camera[i].S().val);
                sserr << ssequal(name,  this->varName("S",i)) << ssthrow;

                name = this->read(is, 9, this->m_camera[i].K().val);
                sserr << ssequal(name, this->varName("K",i)) << ssthrow;

                name = this->read(is, 5, this->m_camera[i].D().val);
                sserr << ssequal(name, this->varName("D",i)) << ssthrow;

                name = this->read(is, 9, this->m_camera[i].R().val);
                sserr << ssequal(name, this->varName("R",i)) << ssthrow;

                name = this->read(is, 3, this->m_camera[i].T().val);
                sserr << ssequal(name, this->varName("T",i)) << ssthrow;

                name = this->read(is, 2, this->m_camera[i].S_rect().val);
                sserr << ssequal(name, this->varName("S_rect",i)) << ssthrow;

                name = this->read(is, 9, this->m_camera[i].R_rect().val);
                sserr << ssequal(name, this->varName("R_rect",i)) << ssthrow;

                name = this->read(is, 12, this->m_camera[i].P_rect().val);
                sserr << ssequal(name, this->varName("P_rect",i)) << ssthrow;

                this->m_camera[i].R_rect_00() = this->m_camera[0].R_rect();
            }

        }

        void Calibration::loadImuToVelo(boost::filesystem::path path) {
            std::ifstream is(path.string());
            std::string name, head;

            // check file path and stream state
            sserr << sspfile(path) << ssthrow;
            sserr << sscond(!is.is_open()) << "cannot open input file stream: " << path << ssthrow;

            // read head line
            std::getline(is,head);
            sserr << sscond(head.empty()) << "error while reading header in file: " << path << ssthrow;

            name = this->read(is, 9, this->m_oxts.R().val);
            sserr << ssequal(name,  "R") << ssthrow;

            name = this->read(is, 3, this->m_oxts.T().val);
            sserr << ssequal(name,  "T") << ssthrow;
        }

        void Calibration::loadVeloToCam(boost::filesystem::path path) {
            std::ifstream is(path.string());
            std::string name, head;

            // check file path and stream state
            sserr << sspfile(path) << ssthrow;
            sserr << sscond(!is.is_open()) << "cannot open input file stream: " << path << ssthrow;

            // read head line
            std::getline(is,head);
            sserr << sscond(head.empty()) << "error while reading header in file: " << path << ssthrow;

            name = this->read(is, 9, this->m_velodyne.R().val);
            sserr << ssequal(name,  "R") << ssthrow;

            name = this->read(is, 3, this->m_velodyne.T().val);
            sserr << ssequal(name,  "T") << ssthrow;

            name = this->read(is, 2, this->m_velodyne.deltaF().val);
            sserr << ssequal(name,  "delta_f") << ssthrow;

            name = this->read(is, 2, this->m_velodyne.deltaC().val);
            sserr << ssequal(name,  "delta_c") << ssthrow;
        }

        Calibration::Calibration(boost::filesystem::path path) {
            this->loadCamToCam(path / "calib_cam_to_cam.txt");
            this->loadImuToVelo(path / "calib_imu_to_velo.txt");
            this->loadVeloToCam(path / "calib_velo_to_cam.txt");
        }

        void Calibration::veloToCam(cv::Vec3f* velo, cv::Vec2f* cam, size_t num, size_t camera) {
            cv::Matx44f TR;
            cv::Matx34f P_rect;
            cv::Matx33f P;

            TR = this->m_camera[camera].TR() * this->m_velodyne.TR();
            P_rect = this->m_camera[camera].P_rect();
            P = cv::Matx33f(
                    P_rect.val[0],P_rect.val[1],P_rect.val[2],
                    P_rect.val[4],P_rect.val[5],P_rect.val[6],
                    P_rect.val[8],P_rect.val[9],P_rect.val[10]);

            cv::Vec4f tmp4;
            cv::Vec3f tmp3;
            cv::Vec2f tmp2;
            for(int i = 0; i < num; i++) {
                tmp4 = TR * cv::Vec4f(velo[i].val[0],velo[i].val[1],velo[i].val[2],1);
                tmp3 = P * cv::Vec3f(tmp4.val[0],tmp4.val[1],tmp4.val[2]);
                tmp2.val[0] = tmp3[0] / tmp3[2];
                tmp2.val[1] = tmp3[1] / tmp3[2];
                cam[i] = tmp2;
            }
        }

        CalibrationReader::CalibrationReader(boost::filesystem::path path) : m_calibration(path / "..")  {
            this->output()->value(&(this->m_calibration));
            this->reset();
        }

        void CalibrationReader::calc() {
            this->output()->value(&(this->m_calibration));
        }

        void CalibrationReader::reset() {
        }
    }
}