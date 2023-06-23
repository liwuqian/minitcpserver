#pragma once
#include "boost/thread.hpp"
#include "boost/bind.hpp"
#include "boost/shared_ptr.hpp"

namespace myspace{
    class MyThread
    {
    public:
        MyThread() : m_bStarted(false){}
        virtual ~MyThread() { Stop(); }

        bool Start()
        {
            if (m_bStarted)
            {
                return false;
            }
            sp_thread.reset(new boost::thread(boost::bind(&MyThread::Run, this), this));
            m_bStarted = true;
            sp_thread->detach();
            return true;
        }

        bool Stop()
        {
            if (m_bStarted)
            {
                sp_thread->interrupt();
                m_bStarted = false;
            }
            return false;
        }

        void Sleep (const unsigned int& ms) {
            boost::this_thread::sleep(boost::posix_time::millisec(ms));
        }

    protected:
        virtual void Run(void) = 0;
        bool m_bStarted;

    private:
        boost::shared_ptr<boost::thread> sp_thread;
    };

}