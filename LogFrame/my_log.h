#pragma once

#define _CRT_SECURE_NO_WARNINGS //disable security error

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "my_log_def.h"


#ifdef BUILD_MY_LOG
#define __SELF
#endif
#include "my_imp_exp.h"

#define WRITE_FILE_MODE_TURNC           0           // delete file and write file 
#define WRITE_FILE_MODE_APP             1           // append file context
#define WRITE_FILE_MODE_END             2           // close file after write file

#define MY_LOG_SPLIT_SYMBOL_ENTER           "\n"
#define MY_LOG_SPLIT_SYMBOL_SPACE           " "
#define MY_LOG_SPLIT_SYMBOL_TABLE           "\t"


namespace myspace {
    const static char MY_LOG_INFO[] = "LOG_INFO";
    const static char MY_LOG_WARNING[] = "LOG_WARNING";
    const static char MY_LOG_ERROR[] = "LOG_ERROR";

    __IM_EX_FUNC_C void MyLog_Initialize();

    __IM_EX_FUNC_C void MyLog_Shutdown();


    struct U8Ptr 
    {
        const char *split;
        uint8_t *pdata;
        uint64_t length;
    };

    struct U16Ptr 
    {
        const char *split;
        uint16_t *pdata;
        uint64_t length;
    };

    struct U32Ptr 
    {
        const char *split;
        uint32_t *pdata;
        uint64_t length;
    };

    struct U64Ptr 
    {
        const char *split;
        uint64_t *pdata;
        uint64_t length;
    };

    struct U8Data 
    {
        uint8_t data;
    };

    struct U16Data 
    {
        uint16_t data;
    };

    struct U32Data 
    {
        uint32_t data;
    };

    struct U64Data 
    {
        uint64_t data;
    };

    class __IM_EX_CLASS MyLog 
    {
    public:
        MyLog(const char* path, const char *file, const char *func, const int32_t &line, const char *logSource, const int32_t &logId, const char *logLevel);
        MyLog(const char *file_path, uint32_t mode);

        ~MyLog();
        MyLog &operator << (const int8_t &val);
        MyLog &operator << (const uint8_t &val);
        MyLog &operator << (const int16_t &val);
        MyLog &operator << (const uint16_t &val);
        MyLog &operator << (const int32_t &val);
        MyLog &operator << (const uint32_t &val);
        MyLog &operator << (const int64_t &val);
        MyLog &operator << (const uint64_t &val);
        MyLog &operator << (const float &val);
        MyLog &operator << (const double &val);
        MyLog &operator << (const bool &val);
        MyLog &operator << (const char *val);
        MyLog &operator << (const U8Ptr &val);
        MyLog &operator << (const U16Ptr &val);
        MyLog &operator << (const U32Ptr &val);
        MyLog &operator << (const U64Ptr &val);
        MyLog &operator << (const U8Data &val);
        MyLog &operator << (const U16Data &val);
        MyLog &operator << (const U32Data &val);
        MyLog &operator << (const U64Data &val);

        template <typename PACKET, typename PDATA>
        static inline PACKET pair(PDATA pdata, uint64_t length, const char *split) 
        {
            PACKET packet;
            packet.split = split;
            packet.pdata = pdata;
            packet.length = length;
            return packet;
        }

        template <typename PACKET, typename DATA>
        static inline PACKET pair(DATA data) 
        {
            PACKET packet;
            packet.data = data;
            return packet;
        }


    private:
        inline bool GetMsgLen(size_t &len);
        template <typename T>
        inline void DoCopy(const char *print_type, const T &val) 
        {
            if(m_Msg.length < (Log::max_message_length - 1)) 
            {
                char pre_check[Log::max_d_length];
                size_t len = _snprintf(&pre_check[0], Log::max_d_length, print_type, val);
                uint16_t start_point = m_Msg.length;
                // if there is enough space for new message
                if(GetMsgLen(len))
                    memcpy(&m_Msg.message[start_point], &pre_check[0], len);
            }
        }

        Log::MsgInfo m_Msg;
    };
}


#ifndef LOG_INIT
#define LOG_INIT              myspace::MyLog_Initialize()
#endif

#ifndef LOG_SHUTDOWN
#define LOG_SHUTDOWN          myspace::MyLog_Shutdown()
#endif

#ifndef LOG_INFO
#define LOG_INFO(path, sSource, uiLogUID)      myspace::MyLog(path, __FILE__, __FUNCTION__, __LINE__, sSource, uiLogUID, myspace::MY_LOG_INFO)
#endif

#ifndef LOG_WARNING
#define LOG_WARNING(path, sSource, uiLogUID)   myspace::MyLog(path, __FILE__, __FUNCTION__, __LINE__, sSource, uiLogUID, myspace::MY_LOG_WARNING)
#endif

#ifndef LOG_ERROR
#define LOG_ERROR(path, sSource, uiLogUID)     myspace::MyLog(path, __FILE__, __FUNCTION__, __LINE__, sSource, uiLogUID, myspace::MY_LOG_ERROR)
#endif


#ifndef LOG_WRITE_BEGIN
#define LOG_WRITE_BEGIN(path)                         myspace::MyLog(path, UMR_WRITE_FILE_MODE_TURNC)
#endif

#ifndef LOG_WRITE_FILE
#define LOG_WRITE_FILE(path)                          myspace::MyLog(path, UMR_WRITE_FILE_MODE_APP)
#endif

#ifndef LOG_WRITE_END
#define LOG_WRITE_END(path)                           myspace::MyLog(path, UMR_WRITE_FILE_MODE_END)
#endif



#define HEX_BYTE(data)                                  myspace::MyLog::pair<myspace::U8Data, uint8_t>((uint8_t)(data))
#define HEX_WORD(data)                                  myspace::MyLog::pair<myspace::U16Data, uint16_t>((uint16_t)(data))
#define HEX_DWORD(data)                                 myspace::MyLog::pair<myspace::U32Data, uint32_t>((uint32_t)(data))
#define HEX_QWORD(data)                                 myspace::MyLog::pair<myspace::U64Data, uint64_t>((uint64_t)(data))

#define HEX_BYTE_ARRAY(pdata, size, split)              myspace::MyLog::pair<myspace::U8Ptr, uint8_t*>((uint8_t*)(pdata), size, split)
#define HEX_WORD_ARRAY(pdata, size, split)              myspace::MyLog::pair<myspace::U16Ptr, uint16_t*>((uint16_t*)(pdata), size, split)
#define HEX_DWORD_ARRAY(pdata, size, split)             myspace::MyLog::pair<myspace::U32Ptr, uint32_t*>((uint32_t*)(pdata), size, split)
#define HEX_QWORD_ARRAY(pdata, size, split)             myspace::MyLog::pair<myspace::U64Ptr, uint64_t*>((uint64_t*)(pdata), size, split)


#define HEX_BYTE_ARRAY_SPACE(pdata, size)               HEX_BYTE_ARRAY(pdata, size, MR_LOG_SPLIT_SYMBOL_SPACE)
#define HEX_WORD_ARRAY_SPACE(pdata, size)               HEX_WORD_ARRAY(pdata, size, MR_LOG_SPLIT_SYMBOL_SPACE)
#define HEX_DWORD_ARRAY_SPACE(pdata, size)              HEX_DWORD_ARRAY(pdata, size, MR_LOG_SPLIT_SYMBOL_SPACE)
#define HEX_QWORD_ARRAY_SPACE(pdata, size)              HEX_QWORD_ARRAY(pdata, size, MR_LOG_SPLIT_SYMBOL_SPACE)

#define HEX_BYTE_ARRAY_ENTER(pdata, size)               HEX_BYTE_ARRAY(pdata, size, MR_LOG_SPLIT_SYMBOL_ENTER)
#define HEX_WORD_ARRAY_ENTER(pdata, size)               HEX_WORD_ARRAY(pdata, size, MR_LOG_SPLIT_SYMBOL_ENTER)
#define HEX_DWORD_ARRAY_ENTER(pdata, size)              HEX_DWORD_ARRAY(pdata, size, MR_LOG_SPLIT_SYMBOL_ENTER)
#define HEX_QWORD_ARRAY_ENTER(pdata, size)              HEX_QWORD_ARRAY(pdata, size, MR_LOG_SPLIT_SYMBOL_ENTER)

#define HEX_BYTE_ARRAY_TABLE(pdata, size)               HEX_BYTE_ARRAY(pdata, size, MR_LOG_SPLIT_SYMBOL_TABLE)
#define HEX_WORD_ARRAY_TABLE(pdata, size)               HEX_WORD_ARRAY(pdata, size, MR_LOG_SPLIT_SYMBOL_TABLE)
#define HEX_DWORD_ARRAY_TABLE(pdata, size)              HEX_DWORD_ARRAY(pdata, size, MR_LOG_SPLIT_SYMBOL_TABLE)
#define HEX_QWORD_ARRAY_TABLE(pdata, size)              HEX_QWORD_ARRAY(pdata, size, MR_LOG_SPLIT_SYMBOL_TABLE)

