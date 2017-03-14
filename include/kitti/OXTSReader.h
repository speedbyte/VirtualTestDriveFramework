//
// Created by geislerd on 13.02.17.
//

#ifndef OXTSREADER_H
#define OXTSREADER_H

#include <io/CSVReader.h>
#include <opencv2/opencv.hpp>

namespace saliency_sandbox {
    namespace kitti {

        typedef typename saliency_sandbox::io::CSVReader<
                ' ',   // separator
                float, // latitude of the oxts-unit (deg)
                float, // longitude of the oxts-unit (deg)
                float, // altitude of the oxts-unit (m)
                float, // roll angle (rad),  0 = level, positive = left side up (-pi..pi)
                float, // pitch angle (rad), 0 = level, positive = front down (-pi/2..pi/2)
                float, // heading (rad),     0 = east,  positive = counter clockwise (-pi..pi)
                float, // velocity towards north (m/s)
                float, // velocity towards east (m/s)
                float, // forward velocity, i.e. parallel to earth-surface (m/s)
                float, // leftward velocity, i.e. parallel to earth-surface (m/s)
                float, // upward velocity, i.e. perpendicular to earth-surface (m/s)
                float, // acceleration in x, i.e. in direction of vehicle front (m/s^2)
                float, // acceleration in y, i.e. in direction of vehicle left (m/s^2)
                float, // acceleration in z, i.e. in direction of vehicle top (m/s^2)
                float, // forward acceleration (m/s^2)
                float, // leftward acceleration (m/s^2)
                float, // upward acceleration (m/s^2)
                float, // angular rate around x (rad/s)
                float, // angular rate around y (rad/s)
                float, // angular rate around z (rad/s)
                float, // angular rate around forward axis (rad/s)
                float, // angular rate around leftward axis (rad/s)
                float, // angular rate around upward axis (rad/s)
                float, // velocity accuracy (north/east in m)
                float, // velocity accuracy (north/east in m/s)
                int,   // navigation status
                int,   // number of satellites tracked by primary GPS receiver
                int,   // position mode of primary GPS receiver
                int,   // velocity mode of primary GPS receiver
                int    // orientation mode of primary GPS receiver
        > OXTSCSVReader;

        class IOXTSRow {

        public:
            /**
             *
             * @return latitude of the oxts-unit (deg)
             */
            virtual float lat() = 0;

            /**
             *
             * @return longitude of the oxts-unit (deg)
             */
            virtual float lon() = 0;

            /**
             *
             * @return altitude of the oxts-unit (m)
             */
            virtual float alt() = 0;

            /**
             *
             * @return roll angle (rad),  0 = level, positive = left side up (-pi..pi)
             */
            virtual float roll() = 0;

            /**
             *
             * @return pitch angle (rad), 0 = level, positive = front down (-pi/2..pi/2)
             */
            virtual float pitch() = 0;

            /**
             *
             * @return heading (rad),     0 = east,  positive = counter clockwise (-pi..pi)
             */
            virtual float yaw() = 0;

            /**
             *
             * @return velocity towards north (m/s)
             */
            virtual float vn() = 0;

            /**
             *
             * @return velocity towards east (m/s)
             */
            virtual float ve() = 0;

            /**
             *
             * @return forward velocity, i.e. parallel to earth-surface (m/s)
             */
            virtual float vf() = 0;

            /**
             *
             * @return leftward velocity, i.e. parallel to earth-surface (m/s)
             */
            virtual float vl() = 0;

            /**
             *
             * @return upward velocity, i.e. perpendicular to earth-surface (m/s)
             */
            virtual float vu() = 0;

            /**
             *
             * @return acceleration in x, i.e. in direction of vehicle front (m/s^2)
             */
            virtual float ax() = 0;

            /**
             *
             * @return acceleration in y, i.e. in direction of vehicle left (m/s^2)
             */
            virtual float ay() = 0;

            /**
             *
             * @return acceleration in z, i.e. in direction of vehicle top (m/s^2)
             */
            virtual float az() = 0;

            /**
             *
             * @return forward acceleration (m/s^2)
             */
            virtual float af() = 0;

            /**
             *
             * @return leftward acceleration (m/s^2)
             */
            virtual float al() = 0;

            /**
             *
             * @return upward acceleration (m/s^2)
             */
            virtual float au() = 0;

            /**
             *
             * @return angular rate around x (rad/s)
             */
            virtual float wx() = 0;

            /**
             *
             * @return angular rate around y (rad/s)
             */
            virtual float wy() = 0;

            /**
             *
             * @return angular rate around z (rad/s)
             */
            virtual float wz() = 0;

            /**
             *
             * @return angular rate around forward axis (rad/s)
             */
            virtual float wf() = 0;

            /**
             *
             * @return angular rate around leftward axis (rad/s)
             */
            virtual float wl() = 0;

            /**
             *
             * @return angular rate around upward axis (rad/s)
             */
            virtual float wu() = 0;

            /**
             *
             * @return velocity accuracy (north/east in m)
             */
            virtual float posacc() = 0;

            /**
             *
             * @return velocity accuracy (north/east in m/s)
             */
            virtual float velacc() = 0;

            /**
             *
             * @return navigation status
             */
            virtual int navstat() = 0;

            /**
             *
             * @return number of satellites tracked by primary GPS receiver
             */
            virtual int numsats() = 0;

            /**
             *
             * @return position mode of primary GPS receiver
             */
            virtual int posmode() = 0;

            /**
             *
             * @return velocity mode of primary GPS receiver
             */
            virtual int velmode() = 0;

            /**
             *
             * @return orientation mode of primary GPS receiver
             */
            virtual int orimode() = 0;
        };

        class OXTSRow : public IOXTSRow, public saliency_sandbox::core::IBus {
        private:
            OXTSCSVReader::OutputBus* m_bus;
        public:

            template<uint32_t _index>
            class Selector : public saliency_sandbox::core::Node::template Input<OXTSRow>::template Output<typename OXTSCSVReader::OutputPortListElement<_index>::type::type> {
            public:
                Selector() {

                    this->template input<0>()->name("OXTS");
                    this->template output<0>()->name("OXTS Channel");
                }
                void calc() override {
                    this->template output<0>()->value(this->template input<0>()->value()->template value<_index>());
                }

                void reset() override {

                }
            };

            OXTSRow(OXTSCSVReader* csv_reader);

            template<uint32_t _index>
            typename OXTSCSVReader::OutputPortListElement<_index>::type::type* value() {
                return this->m_bus->port<_index>()->value();
            }

            float lat() override;

            float lon() override;

            float alt() override;

            float roll() override;

            float pitch() override;

            float yaw() override;

            float vn() override;

            float ve() override;

            float vf() override;

            float vl() override;

            float vu() override;

            float ax() override;

            float ay() override;

            float az() override;

            float af() override;

            float al() override;

            float au() override;

            float wx() override;

            float wy() override;

            float wz() override;

            float wf() override;

            float wl() override;

            float wu() override;

            float posacc() override;

            float velacc() override;

            int navstat() override;

            int numsats() override;

            int posmode() override;

            int velmode() override;

            int orimode() override;

            uint32_t numChannels() override;

            core::IBusChannel *channel(uint32_t index);
        };

        class OXTSReader : public saliency_sandbox::core::Node::template Input<>::template Output<OXTSRow> {
        private:
            boost::filesystem::path m_path;
            OXTSCSVReader m_csv_reader;
            OXTSRow m_row;

            boost::filesystem::path filename();

            boost::filesystem::path filepath();


        public:
            OXTSReader(boost::filesystem::path path);

            void calc() override;

            void reset() override;
        };

    }
}

#endif //OXTSREADER_H
