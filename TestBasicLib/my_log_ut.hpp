#include <iostream>
#include "../LogFrame/my_log.h"
#include <stdio.h>


namespace {
    const char path[20] = "logtest.txt";
    #define LOG_INFO_TEST LOG_INFO(path, "LogTest", 111)
    #define LOG_WARNING_TEST LOG_WARNING(path, "LogTest", 111)
    #define LOG_ERROR_TEST LOG_ERROR(path, "LogTest", 111)
    class LogTest
    {
    public:
        void test()
        {
            LOG_INIT;
            LOG_INFO_TEST << "hello";
            LOG_INFO_TEST << "world";
            LOG_INFO_TEST << 1.23;
            bool b = true;
            LOG_INFO_TEST << b;
            LOG_INFO_TEST << 5000;
            LOG_INFO_TEST << HEX_QWORD(15);
            LOG_INFO_TEST << HEX_BYTE(5);
            LOG_WARNING_TEST << HEX_BYTE(5);
            LOG_ERROR_TEST << HEX_BYTE(5);
            uint32_t iid = 123456;
            LOG_ERROR_TEST << iid;
            LOG_SHUTDOWN;
        }
    };
}