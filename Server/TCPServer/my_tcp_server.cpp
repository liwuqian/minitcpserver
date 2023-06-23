#include "my_tcp_server.h"
#include "my_tcp_command_actor.h"
#include "my_tcp_slave.h"
#include "../../include/Common/tcp_common_def.h"

namespace myspace {

	MySocketServer::MySocketServer()
	{
		m_mastersock = INVALID_SOCKET;
	}

	MySocketServer::~MySocketServer()
	{
		closesocket(m_mastersock);
		WSACleanup();
	}

	void MySocketServer::Initialize()
	{
		WSADATA wsaData;
		if (WSAStartup(WSVERS, &wsaData) != 0)
		{
			std::cout << "WSAStartup error" << std::endl;
			LOG_ERROR_SERVER << "WSAStartup error";
			throw std::string("WSAStartup error");
		}
	}

	void MySocketServer::StartThreadPool(int poolsize)
	{
		m_spThredPool.reset(new ThreadPool(4));
	}

	void MySocketServer::StartCommandActor()
	{
		m_spActor = boost::make_shared<TCPServerCommandActor>();
		m_spActor->Start();
	}

	void MySocketServer::StartServer()
	{
		if (nullptr == m_spThredPool.get())
		{
			LOG_ERROR_SERVER << "m_spThredPool is nullptr";
			throw std::string("m_spThredPool is nullptr");
		}
		struct sockaddr_in clientsin; // the from address of o client
		SOCKET mastersock, slavesock; // master & slave socket
		// create a tcp socket
		mastersock = PassiveTCP();
		if (INVALID_SOCKET == mastersock)
		{
			WSACleanup();
			LOG_ERROR_SERVER << "create socket error";
			throw std::string("create socket error");
		}

		int iAddrlen = sizeof(sockaddr_in);
		// loop for accepting 
		while (true)
		{
			std::cout << "wait for connecting...\n";
			LOG_INFO_SERVER << "wait for connecting...";
			slavesock = accept(mastersock, (SOCKADDR*)&clientsin, &iAddrlen);
			if (slavesock == INVALID_SOCKET)
			{
				LOG_ERROR_SERVER << "accept error";
				std::cout << "accept error !\n";
				continue;
			}
			std::cout << "Conneted£º" << inet_ntoa(clientsin.sin_addr) << " : " << ntohs(clientsin.sin_port) << std::endl;
			LOG_INFO_SERVER << "Conneted£º" << (int)inet_ntoa(clientsin.sin_addr) << " : " << ntohs(clientsin.sin_port);
			// push slavesock to thread pool
			m_spThredPool->enqueue(MySlaveServer(), slavesock, clientsin, m_spActor);
		}
	}

	SOCKET MySocketServer::PassiveTCP(int transport)
	{
		// create socket 
		SOCKET slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (slisten == INVALID_SOCKET)
		{
			std::cout << "create socket invalid" << std::endl;
			LOG_ERROR_SERVER << "create socket invalid";
			return INVALID_SOCKET;
		}

		// bind IP and port
		sockaddr_in sin;
		sin.sin_family = AF_INET;
		sin.sin_port = htons(transport);
		sin.sin_addr.S_un.S_addr = INADDR_ANY;
		if (bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
		{
			std::cout << "bind socket error" << std::endl;
			LOG_ERROR_SERVER << "bind socket error";
			return INVALID_SOCKET;
		}

		// start listenning
		if (listen(slisten, QLEN) == SOCKET_ERROR)
		{
			std::cout << "listen error" << std::endl;
			LOG_ERROR_SERVER << "listen error";
			return INVALID_SOCKET;
		}

		return slisten;
	}

}