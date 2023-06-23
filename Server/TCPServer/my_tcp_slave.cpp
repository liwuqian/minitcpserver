#define _CRT_SECURE_NO_WARNINGS
#include "my_tcp_slave.h"
#include "my_tcp_command_actor.h"
#include "../../include/Common/my_utility.hpp"
#include "../../include/Common/tcp_common_def.h"
#include <sstream>

namespace myspace {

	void MySlaveServer::operator()(SOCKET sock, sockaddr_in fsockin, const boost::shared_ptr<TCPServerCommandActor>& actor)
	{
        char revData[1024];
        std::string toData;
        while (true)
        {           
            // receive msg
            int ret = recv(sock, revData, 1024, 0);
            if (ret > 0)
            {
                revData[ret] = 0x00;
                std::stringstream stid;
                stid << std::this_thread::get_id();
                std::cout << "worker thread ID: " << stid.str() << " Port: " << ntohs(fsockin.sin_port) << std::endl;
                std::cout << revData << std::endl;
                LOG_INFO_SERVER << "worker thread ID: " << stid.str().c_str() << " Port: " 
                    << ntohs(fsockin.sin_port) << " " << revData;
                if (0 == strcmp(ksLove.c_str(), revData))
                {
                    actor->PostCommand(ksLove);
                    toData = Tools::self().GetLoveGraphic();
                }
                else if (0 == strcmp(ksDate.c_str(), revData))
                {
                    toData = GetSysCurrentTime().c_str();
                }
                else if (0 == strcmp(ksTask.c_str(), revData))
                {
                    actor->PostCommand(ksTask);
                    toData = "doing massive calculation...";
                }
                else
                {
                    toData = "hello client";
                }
            }

            // send msg 
            if (SOCKET_ERROR == send(sock, toData.c_str(), toData.length(), 0))
            {
                std::cout << "client " << ntohs(fsockin.sin_port) << " has quit, disconnect !!! \n";
                LOG_INFO_SERVER << "client " << ntohs(fsockin.sin_port) << " has quit, disconnect !!!";
                break;
            }
        }
        closesocket(sock);
	}

    std::string MySlaveServer::GetSysCurrentTime() const
    {
        auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::stringstream ss;
        ss << std::put_time(std::localtime(&t), "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }
    
}