#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <winsock2.h>
#include "../../include/Common/my_thread_pool.hpp"
#include "../../include/Common/tcp_common_def.h"
#include <boost/smart_ptr.hpp>
#include <stdexcept>

namespace myspace {

const int QLEN = 4;

class TCPServerCommandActor;

class MySocketServer
{
public:
    MySocketServer();

    ~MySocketServer();

    void Initialize();

    void StartThreadPool(int poolsize);

    void StartCommandActor();

    void StartServer();

private:
    SOCKET PassiveTCP(int transport = 8888);

    boost::shared_ptr<TCPServerCommandActor> m_spActor;
    boost::shared_ptr<ThreadPool> m_spThredPool;

    SOCKET m_mastersock;
};

} //myspace
