#include "my_log.h"
#include "my_log_def.h"
#include "my_log_task.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/filesystem.hpp>
#include "my_log_queue.h"
#include <fstream>
#include <process.h>

using namespace myspace;
using namespace myspace::Log;

static bool gMRLog_Debug = false;

void myspace::MyLog_Initialize() 
{
    if(boost::filesystem::exists("LogDebug")) 
    {
        gMRLog_Debug = true;
    }
    myspace::LogTask::self().Initialize();
}

MyLog::MyLog(const char* path, const char *file, const char *func, const int32_t &line,
             const char *logSource, const int32_t &logId, const char *logLevel) 
{
    //std::string stime/* = boost::posix_time::to_iso_string(boost::posix_time::second_clock::local_time())*/;
    auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::stringstream ss;
    ss << std::put_time(std::localtime(&t), "%Y-%m-%d %H:%M:%S");
    strncpy_s(m_Msg.time, ss.str().c_str(), max_time_string_length-1);
    strncpy_s(m_Msg.file, file, max_file_string_length-1);
    strncpy_s(m_Msg.func, func, max_function_string_length-1);
    strncpy_s(m_Msg.source, logSource, max_log_source-1);
    strncpy_s(m_Msg.level, logLevel, max_log_level-1);
    strncpy_s(m_Msg.path, path, max_file_path_length-1);
    m_Msg.log_id = logId;
    m_Msg.line = line;
    m_Msg.mode = WRITE_FILE_MODE_APP;
    m_Msg.length = 0;
}

MyLog::MyLog(const char *file_path, uint32_t mode) {
    memset(m_Msg.file, 0, max_file_string_length-1);
    memset(m_Msg.func, 0, max_function_string_length-1);
    memset(m_Msg.source, 0, max_log_source-1);
    memset(m_Msg.level, 0, max_log_level-1);
    strncpy_s(m_Msg.path, file_path, max_file_path_length-1);
    m_Msg.mode = mode;
    m_Msg.length = 0;
}

MyLog::~MyLog() 
{
    if(gMRLog_Debug) 
    {
        std::string sTime/*(boost::posix_time::to_iso_string(boost::posix_time::second_clock::local_time()))*/;
        auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::stringstream ss;
        ss << std::put_time(std::localtime(&t), "%Y-%m-%d %H:%M:%S");
        std::string sFile = "LogDebug/" + sTime.substr(0,11) + ".log";
        std::ofstream fout(sFile, std::ios::app);
        fout <<"["<<boost::posix_time::to_simple_string(boost::posix_time::microsec_clock::local_time())<<"]-"
            <<m_Msg.source<<"-["<<m_Msg.func<<"]-["<<m_Msg.line<<"]-"<<m_Msg.message<< std::endl;
        fout.close();
    }

    m_Msg.pid = _getpid();
    myspace::Log::LogQueue::push(m_Msg);
}

MyLog &MyLog::operator << (const int32_t &val) 
{
    DoCopy("%d", val);
    return *this;
}

MyLog &MyLog::operator << (const int8_t &val) 
{
    DoCopy("%d", val);
    return *this;
}

MyLog &MyLog::operator << (const uint8_t &val) 
{
    DoCopy("%u", val);
    return *this;
}

MyLog &MyLog::operator << (const int16_t &val) 
{
    DoCopy("%d", val);
    return *this;
}

MyLog &MyLog::operator << (const uint16_t &val) 
{
    DoCopy("%u", val);
    return *this;
}

MyLog &MyLog::operator << (const uint32_t &val) 
{
    DoCopy("%u", val);
    return *this;
}

MyLog &MyLog::operator << (const int64_t &val) 
{
    DoCopy("%lld", val);
    return *this;
}

MyLog &MyLog::operator << (const uint64_t &val) 
{
    DoCopy("%llu", val);
    return *this;
}

MyLog &MyLog::operator << (const bool &val) {
    *this << (val ? "TRUE" : "FALSE");
    return *this;
}

MyLog &MyLog::operator << (const float &val) 
{
    DoCopy("%f", val);
    return *this;
}

MyLog &MyLog::operator << (const double &val) 
{
    DoCopy("%lf", val);
    return *this;
}

MyLog &MyLog::operator << (const char *val) 
{
    if(m_Msg.length < (max_message_length - 1) &&  NULL != val) 
    {
        size_t len = strlen(val);
        uint16_t start_point = m_Msg.length;
        // if there is enough space for new message
        if(GetMsgLen(len))
            std::strncpy(&m_Msg.message[start_point], val, len);
    }
    return *this;
}

inline bool MyLog::GetMsgLen(size_t &len) 
{
    // m_length is 0 based, umr_max_message_length is 1 based
    if(m_Msg.length < (max_message_length - 1)) 
    {
        // if there is enough space for new message
        if((m_Msg.length + len) < max_message_length) 
        {
            m_Msg.length += len;
        } 
        else 
        {
            // if there is not enough space, cut the rest!
            len = (max_message_length-1) - m_Msg.length;
            m_Msg.length = max_message_length;
        }
        return true;
    }
    return false;
}


MyLog &MyLog::operator << (const myspace::U8Ptr &val) 
{
    if(val.pdata) 
    {
        char format[32] = "%02x";
        memcpy(format + strlen(format), val.split, 32);
        for(uint64_t idx = 0; idx < val.length; ++idx) 
        {
            DoCopy(format, val.pdata[idx]);
        }
    }
    return *this;
}

MyLog &MyLog::operator << (const myspace::U16Ptr &val) 
{
    if(val.pdata) 
    {
        char format[32] = "%04x";
        memcpy(format + strlen(format), val.split, 32);
        for(uint64_t idx = 0; idx < val.length; ++idx) 
        {
            DoCopy(format, val.pdata[idx]);
        }
    }
    return *this;
}

MyLog &MyLog::operator << (const myspace::U32Ptr &val) 
{
    if(val.pdata) {
        char format[32] = "%08x";
        memcpy(format + strlen(format), val.split, 32);
        for(uint64_t idx = 0; idx < val.length; ++idx) 
        {
            DoCopy(format, val.pdata[idx]);
        }
    }
    return *this;
}

MyLog &MyLog::operator << (const myspace::U64Ptr &val) 
{
    if(val.pdata) 
    {
        char format[32] = "%016llx";
        memcpy(format + strlen(format), val.split, 32);
        for(uint64_t idx = 0; idx < val.length; ++idx) 
        {
            DoCopy(format, val.pdata[idx]);
        }
    }
    return *this;
}

MyLog &MyLog::operator << (const myspace::U8Data &val) 
{
    DoCopy("%02x", val.data);
    return *this;
}

MyLog &MyLog::operator << (const myspace::U16Data &val) 
{
    DoCopy("%04x", val.data);
    return *this;
}

MyLog &MyLog::operator << (const myspace::U32Data &val) 
{
    DoCopy("%08x", val.data);
    return *this;
}

MyLog &MyLog::operator << (const myspace::U64Data &val) 
{
    DoCopy("%016llx", val.data);
    return *this;
}



