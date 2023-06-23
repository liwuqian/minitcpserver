#pragma once
#include <iostream>
#include "boost/thread.hpp"
#include "boost/bind.hpp"
#include "boost/shared_ptr.hpp"
#include "my_thread.hpp"
#include "my_lock_free_queue.hpp"
#include "my_command_actor.hpp"
#include "common_define.h"

namespace myspace{
    class ServerCommandActor : public MyCommandActor<CommandStruct, int>
    {
    public:
        virtual void ParseCommand(const CommandStruct& cmd, int& ret)
        {
            int icmdid = cmd.iCmd;
            std::cout << "receive ID: " << icmdid << std::endl;
            //switch (icmdid)
            //{
            //case 0:
            //    {
            //        std::cout << "receive 0" << std::endl;
            //    }
            //    break;
            //case 1:
            //    {
            //        std::cout << "receive 1" << std::endl;
            //    }
            //    break;
            //default:
            //    std::cout << "unknown id: " << icmdid << std::endl;
            //}
        }
    };

    class CommandActorTest
    {
    public:
        CommandActorTest()
        {
            m_actor = boost::make_shared<ServerCommandActor>();
        }
        void test()
        {           
            m_actor->Start();
            CommandStruct cmd;
            int sum = 0;
            while (sum++ < 10)
            {
                cmd.iCmd = sum;
                m_actor->PostCommand(cmd);
            }           
            //boost::this_thread::sleep(boost::posix_time::millisec(5000));
        }    
    private:
        boost::shared_ptr<ServerCommandActor> m_actor;
    };
}