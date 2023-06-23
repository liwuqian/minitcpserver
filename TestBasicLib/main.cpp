#include <iostream>
#include "../include/Common/common_define.h"
#include "../include/Common/my_command_actor_ut.hpp"
#include "../include/Common/my_lock_free_queue.hpp"
#include "boost/shared_array.hpp"
#include <boost/smart_ptr.hpp>
#include <boost/lockfree/queue.hpp>
#include "../include/Common/my_thread.hpp"
#include "../include/Common/my_command_actor.hpp"
#include "../include/Common/my_thread_pool.hpp"
#include "../include/Common/my_thread_pool_ut.hpp"
#include "my_log_ut.hpp"

using namespace myspace;


typedef MyLockfreeQueue<CommandStruct, 2> FreeQ;

template<typename T>
struct Test
{
    boost::lockfree::queue<T, boost::lockfree::capacity<1024>> m_queue;
};

int main()
{
    // test thread pool
    //ThreadPoolTest pooltest;
    //pooltest.test4();

    //// test command actor
    //CommandActorTest actortest;
    //actortest.test();

    // test log
    LogTest logtest;
    logtest.test();

    
    system("pause");

    return 0;
}