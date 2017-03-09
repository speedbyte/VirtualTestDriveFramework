//
// Created by geislerd on 13.02.17.
//

#include <kitti/KittiReader.h>

namespace saliency_sandbox {
    namespace kitti {
        OXTSRow::OXTSRow(OXTSCSVReader* csv_reader) : m_bus(csv_reader->outputBus()) {
            this->m_bus->port<0>()->name("lat");
            this->m_bus->port<1>()->name("lon");
            this->m_bus->port<2>()->name("alt");
            this->m_bus->port<3>()->name("roll");
            this->m_bus->port<4>()->name("pitch");
            this->m_bus->port<5>()->name("yaw");
            this->m_bus->port<6>()->name("vn");
            this->m_bus->port<7>()->name("ve");
            this->m_bus->port<8>()->name("vf");
            this->m_bus->port<9>()->name("vl");
            this->m_bus->port<10>()->name("vu");
            this->m_bus->port<11>()->name("ax");
            this->m_bus->port<12>()->name("ay");
            this->m_bus->port<13>()->name("az");
            this->m_bus->port<14>()->name("af");
            this->m_bus->port<15>()->name("al");
            this->m_bus->port<16>()->name("au");
            this->m_bus->port<17>()->name("wx");
            this->m_bus->port<18>()->name("wy");
            this->m_bus->port<19>()->name("wz");
            this->m_bus->port<20>()->name("wf");
            this->m_bus->port<21>()->name("wl");
            this->m_bus->port<22>()->name("wu");
            this->m_bus->port<23>()->name("posacc");
            this->m_bus->port<24>()->name("velacc");
            this->m_bus->port<25>()->name("navstat");
            this->m_bus->port<26>()->name("numsats");
            this->m_bus->port<27>()->name("posmode");
            this->m_bus->port<28>()->name("velmode");
            this->m_bus->port<29>()->name("orimode");
        }

        float OXTSRow::lat() {
            return *this->value<0>();
        }

        float OXTSRow::lon() {
            return *this->value<1>();
        }

        float OXTSRow::alt() {
            return *this->value<2>();
        }

        float OXTSRow::roll() {
            return *this->value<3>();
        }

        float OXTSRow::pitch() {
            return *this->value<4>();
        }

        float OXTSRow::yaw() {
            return *this->value<5>();
        }

        float OXTSRow::vn() {
            return *this->value<6>();
        }

        float OXTSRow::ve() {
            return *this->value<7>();
        }

        float OXTSRow::vf() {
            return *this->value<8>();
        }

        float OXTSRow::vl() {
            return *this->value<9>();
        }

        float OXTSRow::vu() {
            return *this->value<10>();
        }

        float OXTSRow::ax() {
            return *this->value<11>();
        }

        float OXTSRow::ay() {
            return *this->value<12>();
        }

        float OXTSRow::az() {
            return *this->value<13>();
        }

        float OXTSRow::af() {
            return *this->value<14>();
        }

        float OXTSRow::al() {
            return *this->value<15>();
        }

        float OXTSRow::au() {
            return *this->value<16>();
        }

        float OXTSRow::wx() {
            return *this->value<17>();
        }

        float OXTSRow::wy() {
            return *this->value<18>();
        }

        float OXTSRow::wz() {
            return *this->value<19>();
        }

        float OXTSRow::wf() {
            return *this->value<20>();
        }

        float OXTSRow::wl() {
            return *this->value<21>();
        }

        float OXTSRow::wu() {
            return *this->value<22>();
        }

        float OXTSRow::posacc() {
            return *this->value<23>();
        }

        float OXTSRow::velacc() {
            return *this->value<24>();
        }

        int OXTSRow::navstat() {
            return *this->value<25>();
        }

        int OXTSRow::numsats() {
            return *this->value<26>();
        }

        int OXTSRow::posmode() {
            return *this->value<27>();
        }

        int OXTSRow::velmode() {
            return *this->value<28>();
        }

        int OXTSRow::orimode() {
            return *this->value<29>();
        }

        uint32_t OXTSRow::numChannels() {
            return this->m_bus->numChannels();
        }

        core::IBusChannel *OXTSRow::channel(uint32_t index) {
            return this->m_bus->channel(index);
        }

        boost::filesystem::path OXTSReader::filename() {
            char filename[16];

            sprintf(filename,"%010ld.txt",this->time());

            return boost::filesystem::path(filename);
        }

        boost::filesystem::path OXTSReader::filepath() {
            return this->m_path / "oxts" / "data" / this->filename();
        }

        OXTSReader::OXTSReader(boost::filesystem::path path) : m_path(path), m_row(&(this->m_csv_reader)) {
            this->output<0>()->name("OXTS");
            this->output<0>()->value(&(this->m_row));
        }

        void OXTSReader::calc() {
            boost::filesystem::path path;

            path = this->filepath();
            if(boost::filesystem::exists(path) || boost::filesystem::is_regular_file(path)) {
                this->m_csv_reader.open(this->filepath());
                this->m_csv_reader.calc();
                if (!this->m_csv_reader.eof())
                    this->output<0>()->value(&(this->m_row));
                else
                    this->eof(true);
            } else {
                this->eof(true);
                //TODO: error handling - may end of file list
            }
        }

        void OXTSReader::reset() {
        }
    }
}
