#include <iostream>
#include <fstream>
#include <sstream>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "my_log.h"
#include "my_log_task.h"
#include "my_log_queue.h"

using namespace myspace;
using namespace myspace::Log;

std::map<std::string, std::ofstream *> ProcessLogging::s_vFiles;
static bool umr_log_started = true;
static bool umr_log_finalized = false;


void ProcessLogging::WriteMsg(const MsgInfo &msg) const 
{
    std::map<std::string, std::ofstream *>::iterator iter = s_vFiles.find(msg.path);
    if(WRITE_FILE_MODE_APP == msg.mode)
    {
        if(s_vFiles.end() == iter) 
        {
            std::ofstream *pFile = new std::ofstream(msg.path, std::ios::app);
            s_vFiles.insert(std::pair<std::string, std::ofstream *>(msg.path, pFile));
            iter = s_vFiles.find(msg.path);
        }
        if((NULL != iter->second) && (iter->second->is_open())) 
        {
            Write2file(*(iter->second), msg);
        }
    } 
    else if(WRITE_FILE_MODE_END == msg.mode) 
    {
        if(s_vFiles.end() != iter) 
        {
            if ((NULL != iter->second) && (iter->second->is_open())) 
            {
                iter->second->close();
                delete iter->second;
            }
            s_vFiles.erase(iter);
        }
    } 
    else if(WRITE_FILE_MODE_TURNC == msg.mode) 
    {
        if(s_vFiles.end() == iter) 
        {
            std::ofstream *pFile = new std::ofstream(msg.path, std::ios::out | std::ios::trunc);
            s_vFiles.insert(std::pair<std::string, std::ofstream *>(msg.path, pFile));
            iter = s_vFiles.find(msg.path);
        }
        if((0 != msg.length) && (NULL != iter->second) && (iter->second->is_open())) 
        {
            Write2file(*(iter->second), msg);
        }
    }
}

void ProcessLogging::Write2file(std::ofstream& of, const MsgInfo& msg) const
{
    of << msg.level << " " << msg.time << " " << msg.source << " pid: " << msg.pid 
        << " " << msg.func << ": " << msg.line << " " << msg.message << std::endl;
}

bool ProcessLogging::ReadQueue() 
{
    myspace::Log::MsgInfo message;
    if(myspace::Log::LogQueue::pop(message)) 
    {
        WriteMsg(message);
        return true;
    }
    return false;
}


void myspace::MyLog_Shutdown() 
{
    umr_log_started = false;
    while(!umr_log_finalized) 
    {
        boost::this_thread::sleep(boost::posix_time::millisec(sleep_for_logging));
    }
}

void ProcessLogging::Run(void) 
{
    s_vFiles.clear();
    while(umr_log_started) 
    {
        if(!ReadQueue())
        {
            // if the queue is empty, sleep for 10ms. 
            boost::this_thread::sleep(boost::posix_time::millisec(sleep_for_logging));
        }
    }
    // after someone called shutdown
    while(true) 
    {
        if(!ReadQueue())
            break;
    }
    umr_log_finalized = true;
}


bool LogTask::m_LogTaskInitialized = false;

void LogTask::Initialize() 
{
    if(!m_LogTaskInitialized) 
    {
        Start();
        m_LogTaskInitialized = true;
    }
}

void LogTask::Start(void) 
{
    m_pl.Start();
}

LogTask::~LogTask() 
{
    m_pl.Stop();
}


void LogTask::Stop(void) 
{
    // DO NOT SUPPORT IT
}

