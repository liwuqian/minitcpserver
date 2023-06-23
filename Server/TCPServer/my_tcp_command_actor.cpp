#include "my_tcp_command_actor.h"
#include <iostream>
#include "../../include/Common/my_utility.hpp"
#include "../../include/Common/tcp_common_def.h"

namespace myspace {

	void TCPServerCommandActor::ParseCommand(const std::string& cmd, int& ret)
	{
		if (0 == strcmp(ksLove.c_str(), cmd.c_str()))
		{
			std::cout << "Server Command Actor: " << Tools::self().GetLoveGraphic();
			LOG_INFO_SERVER << "Server Command Actor: " << Tools::self().GetLoveGraphic().c_str();
		}
		else if (0 == strcmp(ksTask.c_str(), cmd.c_str()))
		{
			WaitMoment();
		}
		else
		{
			std::cout << "Server Command Actor: Unknown msg: " << cmd << std::endl;
			LOG_INFO_SERVER << "Server Command Actor: Unknown msg: " << cmd.c_str();
		}
	}
	void TCPServerCommandActor::WaitMoment()
	{
		std::cout << "Server Command Actor: \n" << "->begin doing business \n";
		LOG_INFO_SERVER << "Server Command Actor";
		LOG_INFO_SERVER << "->begin doing business";
		int itimes = 1000;
		while (itimes--)
		{
			std::cout << "*";
			std::this_thread::sleep_for(std::chrono::milliseconds(2));
			if (0 == itimes % 100)
			{
				std::cout << std::endl;
			}
		}
		std::cout << "<-finish doing business \n";
		LOG_INFO_SERVER << "<-finish doing business";
	}
}