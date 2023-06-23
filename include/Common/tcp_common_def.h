#pragma once
#include <string>
#include "../../LogFrame/my_log.h"
#include <stdexcept>
#include <string>

namespace myspace{
    #define WSVERS MAKEWORD(2, 2)

    // Define Server Log
    const char SERVERPATH[20]   = "log_server.txt";
    const char SERVERSOURCE[20] = "log_server";
    const int  SERVERID         = 2000;
    #define LOG_INFO_SERVER     LOG_INFO(SERVERPATH, SERVERSOURCE, 2000)
    #define LOG_WARNING_SERVER  LOG_WARNING(SERVERPATH, SERVERSOURCE, SERVERID)
    #define LOG_ERROR_SERVER    LOG_ERROR(SERVERPATH, SERVERSOURCE, SERVERID)

    // Define Client Log
    const char CLIENTPATH[20]   = "log_clinet.txt";
    const char CLIENTSOURCE[20] = "client";
    const int  CLIENTID         = 2001;
    #define LOG_INFO_CLIENT     LOG_INFO(CLIENTPATH, CLIENTSOURCE, CLIENTID)
    #define LOG_WARNING_CLIENT  LOG_WARNING(CLIENTPATH, CLIENTSOURCE, CLIENTID)
    #define LOG_ERROR_CLIENT    LOG_ERROR(CLIENTPATH, CLIENTSOURCE, CLIENTID)

    // Define Client1 Log
    const char CLIENTPATH2[20]   = "log_client2.txt";
    const char CLIENTSOURCE2[20] = "client2";
    const int  CLIENTID2         = 2002;
    #define LOG_INFO_CLIENT2     LOG_INFO(CLIENTPATH2, CLIENTSOURCE2, CLIENTID2)
    #define LOG_WARNING_CLIENT2  LOG_WARNING(CLIENTPATH2, CLIENTSOURCE2, CLIENTID2)
    #define LOG_ERROR_CLIENT2    LOG_ERROR(CLIENTPATH2, CLIENTSOURCE2, CLIENTID2)

    const std::string ksConnect = "connect";
    const std::string ksDate = "date";
    const std::string ksLove = "love";
    const std::string ksTask = "task";

}