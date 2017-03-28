//
// Created by geislerd on 07.02.17.
//

#ifndef NODE_H
#define NODE_H

#include <core/Port.h>
#include <core/Utils.h>
#include <tuple>
#include <chrono>

namespace saliency_sandbox {

    namespace core {

        template<typename _type>
        class Port;

        template<typename... _type>
        class PortList;

        class Node {
        public:
            template<typename... _type_input>
            class Input {
            public:

                template<uint32_t _index>
                class InputPortListElement {
                public:
                    typedef typename std::tuple_element<_index, std::tuple<_type_input...>>::type inner_type;
                    typedef Port<inner_type> type;
                };

                template<typename... _type_output>
                class Output : public INode {
                public:
                    typedef PortList<_type_input...> InputBus;
                    typedef PortList<_type_output...> OutputBus;

                private:

                    InputBus m_input;
                    OutputBus m_output;
                    time_t m_current;
                    PropertyMap m_properties;
                    bool m_eof;
                    std::string m_name;
                    float m_fps;

                protected:

                    time_t proctime() {
                        return this->m_current;
                    }

                public:

                    Output() : INode(), m_input(this), m_output(this), m_current(-1), m_eof(false), m_fps(0.0f) {

                    }

                    template<uint32_t _index>
                    class OutputPortListElement {
                    public:
                        typedef typename std::tuple_element<_index, std::tuple<_type_output...>>::type inner_type;
                        typedef Port<inner_type> type;
                    };

                    virtual uint32_t numInput() override {
                        return sizeof...(_type_input);
                    }

                    virtual IPort *input(uint32_t index) override {
                        return this->m_input.port(index);
                    }

                    virtual uint32_t numOutput() override {
                        return sizeof...(_type_output);
                    }

                    virtual IPort *output(uint32_t index) override {
                        return this->m_output.port(index);
                    }

                    virtual void process(time_t time) override {
                        std::chrono::high_resolution_clock::time_point t1, t2;
                        long microseconds;
                        bool refresh_fps;

                        this->m_input.process(time);

                        refresh_fps = false;

                        if (time < 0) {
                            this->m_current++;
                            t1 = std::chrono::high_resolution_clock::now();
                            this->calc();
                            t2 = std::chrono::high_resolution_clock::now();
                            refresh_fps = true;
                        } else
                            while(this->m_current < time) {
                                this->m_current++;
                                t1 = std::chrono::high_resolution_clock::now();
                                this->calc();
                                t2 = std::chrono::high_resolution_clock::now();
                                refresh_fps = true;
                            }

                        if(refresh_fps) {
                            microseconds = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
                            if(microseconds)
                                this->m_fps = (1000000.0f) / float(microseconds);
                            else
                                this->m_fps = 0;
                        }
                    }

                    time_t time() override {
                        return this->m_current;
                    }

                    PropertyMap* properties() override {
                        return &(this->m_properties);
                    }

                    const char *name() override {
                        return this->m_name.c_str();
                    }

                    void name(const char* name) override {
                        this->m_name = std::string(name);
                    }

                    float fps() override {
                        return this->m_fps;
                    }

                    virtual void calc() = 0;

                    virtual void reset() = 0;

                    void eof(bool eof) {
                        this->m_eof = eof;
                    }

                    bool eof() {
                        return this->m_eof || this->m_input.eof();
                    }

                    OutputBus* outputBus() {
                        return &(this->m_output);
                    }

                    InputBus* inputBus() {
                        return &(this->m_input);
                    }

                    template<uint32_t _index = 0>
                    typename InputPortListElement<_index>::type *input() {
                        return (typename InputPortListElement<_index>::type *) this->m_input.port(_index);
                    }

                    template<uint32_t _index = 0>
                    typename OutputPortListElement<_index>::type *output() {
                        return (typename OutputPortListElement<_index>::type *) this->m_output.port(_index);
                    }
                };
            };
        };

    }

}

#endif //NODE_H
