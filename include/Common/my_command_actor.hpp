#pragma once

#include "boost/thread.hpp"
#include "boost/bind.hpp"
#include "boost/shared_ptr.hpp"
#include "my_thread.hpp"
#include "my_lock_free_queue.hpp"
#include "common_define.h"

namespace myspace{
    
    template<typename COMMAND, typename RETURN>
    class MyCommandActor : public MyThread
    {
        typedef MyLockfreeQueue<COMMAND, 512> CommandQueue;
    public:
        MyCommandActor()
        {
            m_spQueue.reset(new CommandQueue());
        }
        ~MyCommandActor() {} 

        // sync
        void SendCommand(const COMMAND cmd, RETURN& ret)
        {
            // todo
        }

        // async
        bool PostCommand(const COMMAND cmd)
        {
            if (!m_spQueue->push(cmd))
            {
                Sleep(100);
                return m_spQueue->push(cmd);
            }
            return true;
        }

    protected:
        // ParseCommand must be implementation in derived class
        virtual void ParseCommand(const COMMAND& cmd, RETURN& ret) = 0;

        virtual void Run(void)
        {
            COMMAND oCommand;
            RETURN oReturn;

            while(m_bStarted)
            {
                if( m_spQueue->pop(oCommand) )
                {
                    // parse the received command
                    ParseCommand( oCommand, oReturn );
                }
                else
                {
                    Sleep(100);
                }
            }
        }

    private:
        boost::shared_ptr<CommandQueue> m_spQueue;
    };

}