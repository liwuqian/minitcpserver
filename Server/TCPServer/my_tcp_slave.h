#pragma once
#include <winsock2.h>
#include <boost/smart_ptr.hpp>
#include <string>
#include <stdio.h>
#include <math.h>
#include <sstream>

namespace myspace {
    class TCPServerCommandActor;
    
    class MySlaveServer
    {
    public:
        // slave class functor
        void operator()(SOCKET sock, sockaddr_in fsockin, const boost::shared_ptr<TCPServerCommandActor>& actor);

    private:
        std::string GetSysCurrentTime() const;
    };
}
