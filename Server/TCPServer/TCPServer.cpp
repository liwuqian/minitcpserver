// TCPServer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include "my_tcp_server.h"
#include <boost/smart_ptr.hpp>

#pragma comment(lib,"ws2_32.lib")
using namespace myspace;

int main(int argc, char* argv[])
{
    try
    {
        LOG_INIT;
        boost::shared_ptr<MySocketServer> spserver = boost::make_shared<MySocketServer>();
        spserver->Initialize();
        spserver->StartThreadPool(5);
        spserver->StartCommandActor();
        // loop
        spserver->StartServer();
        LOG_SHUTDOWN;
    }
    catch (const std::exception&)
    {
        std::cout << "server crash\n";
        LOG_ERROR_SERVER << "server crash";
        LOG_SHUTDOWN;
    }

    return 0;
}
