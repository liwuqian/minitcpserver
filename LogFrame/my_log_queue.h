#pragma once

#include <boost/noncopyable.hpp>
#include "my_log_def.h"
#include <boost/lockfree/queue.hpp>

namespace myspace {
    namespace Log {
        class LogQueue : private boost::noncopyable {
        public:
            static bool push(const myspace::Log::MsgInfo &msg) {
                return m_queue.push(msg);
            }
            static bool pop(myspace::Log::MsgInfo &msg) {
                return m_queue.pop(msg);
            }
        private:
            static boost::lockfree::queue<myspace::Log::MsgInfo, boost::lockfree::capacity<myspace::Log::log_buffer_size>> m_queue;
        };
    }
}


