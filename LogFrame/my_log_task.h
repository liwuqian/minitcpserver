#pragma once

#include <map>
#include "../include/Common/my_thread.hpp"
#include "my_log_def.h"
#include <fstream>
#include "../include/Common/my_utility.hpp"

namespace myspace {
    namespace Log {
        class ProcessLogging: public MyThread
        {
        private:
            virtual void Run(void);
            void WriteMsg(const MsgInfo &msg) const;
            void Write2file(std::ofstream &of, const MsgInfo &msg) const;
            bool ReadQueue();
        private:
            static std::map<std::string, std::ofstream *> s_vFiles;
        };
    }

    class LogTask: public Singleton<LogTask> {
    public:
        void Initialize();

        ~LogTask();
    private:
        void Start(void);
        void Stop(void);
        Log::ProcessLogging m_pl;
        static bool m_LogTaskInitialized;
    };
}

