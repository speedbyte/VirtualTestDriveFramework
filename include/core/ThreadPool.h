//
// Created by geislerd on 09.03.17.
//

#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <stdint-gcc.h>
#include <boost/asio/io_service.hpp>
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>

namespace saliency_sandbox {
    namespace core {

        template<uint32_t _n>
        class _ThreadPool {
        protected:
            boost::asio::io_service m_io_service;
            boost::thread_group m_threadpool;

            boost::asio::io_service::work m_work;

        public:
            _ThreadPool() : m_io_service(), m_threadpool(), m_work(m_io_service) {
                for(int i = 0; i < _n; i++)
                    this->m_threadpool.create_thread(
                            boost::bind(&boost::asio::io_service::run, &this->m_io_service)
                    );
            }

            virtual ~_ThreadPool() {
                this->m_io_service.stop();
                this->m_threadpool.join_all();
            }

            boost::asio::io_service& io() {
                return this->m_io_service;
            }
        };

        typedef _ThreadPool<8> ThreadPool;
    }
}

#endif //THREADPOOL_H
