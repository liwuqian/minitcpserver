#include "my_log_queue.h"

boost::lockfree::queue<myspace::Log::MsgInfo, boost::lockfree::capacity<myspace::Log::log_buffer_size>> myspace::Log::LogQueue::m_queue;
