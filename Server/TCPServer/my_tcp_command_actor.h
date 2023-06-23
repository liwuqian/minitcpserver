#pragma once
#include "../../include/Common/my_command_actor.hpp"

namespace myspace {

	class TCPServerCommandActor : public MyCommandActor<std::string, int>
	{
	public:
		virtual void ParseCommand(const std::string& cmd, int& ret);

	private:
		void WaitMoment();
	};
}