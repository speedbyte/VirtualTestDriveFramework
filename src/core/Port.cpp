//
// Created by geislerd on 25.02.17.
//

#include <core/Port.h>

namespace saliency_sandbox {
    namespace core {

        void process_port_thread(IPort *p, time_t time, bool* join) {
            p->process(time);
            *join = true;
        }

        ProcessPortThread::ProcessPortThread(IPort *port, time_t time, bool *join) : m_port(port), m_time(time), m_join(join) {

        }

        ThreadPool ProcessPortThread::thread_pool;

        void ProcessPortThread::run(IPort *port, time_t time, bool *join) {
            thread_pool.io().post(boost::bind(ProcessPortThread::_run,new ProcessPortThread(port,time,join)));
        }

        void ProcessPortThread::_run(ProcessPortThread * arg) {
            arg->m_port->process(arg->m_time);
            *arg->m_join = true;
            delete arg;
        }
    }
}