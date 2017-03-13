//
// Created by geislerd on 07.02.17.
//

#ifndef PORT_H
#define PORT_H

#include <tuple>
#include <core/Processable.h>
#include <core/Node.h>
#include <core/Bus.h>
#include <core/Utils.h>
#include <utils/Error.h>
#include <future>
#include <core/ThreadPool.h>

#define connect_port( source , source_port , target , target_port ) (target).template input<(target_port)>()->connect((source).template output<(source_port)>())

namespace saliency_sandbox {

    namespace core {

        class INode;

        class IPort : public IProcessable , public IBusChannel {
        public:

            virtual IPort *connect() = 0;

            virtual void connect(IPort *port) = 0;

            virtual IProcessable* dependency() = 0;

            virtual INode* node() = 0;
        };

        template<typename _type>
        class Port : public IPort {
        public:
            typedef _type type;
        private:
            std::string m_name;
            type *m_data;
            Port<type> *m_connect;
            INode* m_node;
            bool m_check_port_size;

        public:
            Port(INode* node) :
                    IPort(),
                    m_name(Utils::uniqueName()),
                    m_data(nullptr),
                    m_connect(nullptr),
                    m_node(node),
                    m_check_port_size(true) {

            }

            bool checkPortSize() {
                return this->m_check_port_size;
            }

            void checkPortSize(bool check_port_size) {
                this->m_check_port_size = check_port_size;
            }

            const char *name() override {
                return this->m_name.c_str();
            }

            void name(const char *name) override {
                this->m_name = name;
            }

            size_t size() override {
                return TypeInfo<_type>::SIZE;
            }

            void *data() override {
                return this->m_data;
            }

            void data(void *ptr) override {
                this->m_data = (type *) ptr;
            }

            type *value() {
                return this->m_data;
            }

            void value(type *data) {
                this->data(data);
            }

            IPort *connect() override {
                return this->m_connect;
            }

            void connect(IPort *port) override {

                // check port size if
                // m_check_port_size is true
                sserr << sscond(this->m_check_port_size) << sscond(port->size() != this->size()) << "cannot connect ports with different data sizes" << ssthrow;

                this->m_connect = (Port<type> *) port;
            }

            IProcessable* dependency() override {
                size_t n;

                // return connceted port as dependency if set
                if (this->m_connect != nullptr) {
                    return this->m_connect;
                }
                // return node as dependency if set
                else if (this->m_node != nullptr) {
                    // check for circular dependencies
                    // be careful. this check does not cover all cases
                    n = this->m_node->numInput();
                    for(int i = 0; i < n; i++)
                        sserr << sscond(this->m_node->input(i) == this)
                              << "Circular dependencies detected. This may happen if an input port is not connected. Port name: \"" << this->name() << "\""
                              << ssthrow;
                    return this->m_node;
                }

                sserr << "Port is not connected to another port or node. Port name: Port name: \"" << this->name() << "\"" << ssthrow;
            }

            void process(time_t time) override {
                if(this->dependency()->time() < time || this->dependency()->time() < 0 || time < 0)
                    this->dependency()->process(time);
                if (this->m_connect != nullptr)
                    this->m_data = this->m_connect->m_data;
            }

            float fps() override {
                return this->dependency()->fps();
            }

            time_t time() override {
                return this->dependency()->time();
            }

            INode *node() override {
                return this->m_node;
            }

            bool eof() override {
                return this->dependency()->eof();
            }
        };

        class IPortListElement {
        public:
            virtual IPortListElement *next() = 0;

            virtual IPort *port() = 0;

            virtual IBusChannel *channel() = 0;
        };

        class ProcessPortThread {
        private:
            IPort* m_port;
            time_t m_time;
            bool* m_join;

            static ThreadPool thread_pool;

            static void _run(ProcessPortThread*);

        public:
            ProcessPortThread(IPort *port, time_t time, bool *join);

            static void run(IPort *port, time_t time, bool *join);
        };
        void process_port_thread(IPort* p, time_t time);

        template<uint32_t _index, typename _type, typename... _type_next>
        class PortListElement : public IPortListElement {
        private:
            PortListElement<_index - 1, _type_next...> m_next;
            Port<_type> m_port;

        public:
            PortListElement(INode* node) : m_next(node), m_port(node) {

            }

            IPortListElement *next() override {
                return &(this->m_next);
            }

            IPort *port() override {
                return &(this->m_port);
            }

            IBusChannel *channel() override {
                return &(this->m_port);
            }

            void process(time_t time) {
                //bool join;

                //join = false;
                //ProcessPortThread::run(&(this->m_port),time,&join);
                this->m_port.process(time);
                this->m_next.process(time);

                //while(!join)
                //    usleep(1);
            }

            bool eof() {
                return this->m_port.eof() || m_next.eof();
            }
        };

        template<typename _type>
        class PortListElement<1, _type> : public IPortListElement {

        private:
            Port<_type> m_port;

        public:
            PortListElement(INode* node) : m_port(node) {

            }

            IPortListElement *next() override {
                return nullptr;
            }

            IPort *port() override {
                return &(this->m_port);
            }

            IBusChannel *channel() override {
                return &(this->m_port);
            }

            void process(time_t time) {
                this->m_port.process(time);
            }

            bool eof() {
                return this->m_port.eof();
            }

        };

        template<typename... _type>
        class PortList : public IBus {
        public:
            template<uint32_t _index>
            class PortListElementType {
            public:
                typedef Port<typename std::tuple_element<_index, std::tuple<_type...>>::type> type;
            };
        private:
            PortListElement<sizeof...(_type), _type...> m_element;
            IPortListElement *m_ports[sizeof...(_type)];
        public:

            PortList(INode* node) : m_element(node) {
                this->m_ports[0] = &(this->m_element);
                for (unsigned int index = 1; index < sizeof...(_type); index++)
                    this->m_ports[index] = this->m_ports[index - 1]->next();
            }

            void process(time_t time) {
                this->m_element.process(time);
            }

            bool eof() {
                return this->m_element.eof();
            }

            uint32_t numChannels() override {
                return sizeof...(_type);
            }

            template<uint32_t _index>
            Port<typename std::tuple_element<_index, std::tuple<_type...>>::type> *port() {
                sserr << sscond(_index >= sizeof...(_type)) << "Port index out of bounds. Number of ports: " << sizeof...(_type) << ", Requested index: " << _index << ssthrow;

                return (Port<typename std::tuple_element<_index, std::tuple<_type...>>::type> *) this->m_ports[_index]->port();
            }

            IPort *port(uint32_t index) {
                sserr << sscond(index >= sizeof...(_type)) << "Port index out of bounds. Number of ports: " << sizeof...(_type) << ", Requested index: " << index << ssthrow;

                return this->m_ports[index]->port();
            }

            IPort *channel(uint32_t index) override {
                sserr << sscond(index >= sizeof...(_type)) << "Channel index out of bounds. Number of channels: " << sizeof...(_type) << ", Requested index: " << index << ssthrow;

                return this->m_ports[index]->port();
            }
        };

        template<>
        class PortList<> : public IBus {
        public:

            PortList(INode* node) {
            }

            void process(time_t time) {
            }

            bool eof() {
                return false;
            }

            IPort *port(uint32_t index) {
                sserr << "Port index out of bounds. Number of ports: 0, Requested index: " << index << ssthrow;
            }

            IPort *channel(uint32_t index) override {
                sserr << "Channel index out of bounds. Number of channels: 0, Requested index: " << index << ssthrow;
            }

            uint32_t numChannels() override {
                return 0;
            }
        };

    }

}

#endif //PORT_H
