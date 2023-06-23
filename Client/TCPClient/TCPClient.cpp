// TCPClient.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <iostream>
#include <string>
#include <stdexcept>
#include "../../include/Common/tcp_common_def.h"

using namespace std;
using namespace myspace;
#pragma comment(lib, "ws2_32.lib")

int main()
{
    LOG_INIT;
    try
    {
        WSADATA data;
        if (WSAStartup(WSVERS, &data) != 0)
        {
            throw std::string("1");
        }

        while (true)
        {
            std::string data;
            getline(std::cin, data);
            const char* msg = data.c_str();
            if (0 == strcmp("q", msg) || 0 == strcmp("quit", msg))
            {
                // quit
                break;
            }
            if (0 != strcmp(ksDate.c_str(), msg) && 0 != strcmp(ksLove.c_str(), msg) && 0 != strcmp(ksTask.c_str(), msg))
            {
                std::cout << "please input: date, love or task" << std::endl;
                LOG_INFO_CLIENT << "please input: data love or task";
                continue;
            }
            // create a socket
            SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            if (clientSocket == INVALID_SOCKET)
            {
                cout << "Socket error" << endl;
                LOG_ERROR_CLIENT << "Socket error";
                continue;
            }
            // connect server according to IP:Port
            sockaddr_in sock_in;
            sock_in.sin_family = AF_INET;
            sock_in.sin_port = htons(8888);
            sock_in.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
            if (connect(clientSocket, (sockaddr*)&sock_in, sizeof(sock_in)) == SOCKET_ERROR)
            {
                cout << "Connect error" << endl;
                LOG_ERROR_CLIENT << "Connect error";
                continue;
            }
            //std::cout << "Connect Demo Server Success!\n";

            if (SOCKET_ERROR == send(clientSocket, msg, strlen(msg), 0))
            {
                std::cout << "can not connect to server !!! \n";
                LOG_ERROR_CLIENT << "can not connect to server !!!";
                continue;
            }

            char revdata[1024];
            int num = recv(clientSocket, revdata, 1024, 0);
            if (num > 0)
            {
                revdata[num] = '\0';
                cout << "Sever: " << revdata << endl;
                LOG_INFO_CLIENT << "Sever: " << revdata;
            }

            closesocket(clientSocket);
        }
    }
    catch (const std::string& e)
    {
        std::cout << "crash exit\n";
        LOG_ERROR_CLIENT << "Crash: " << e.c_str();
    }
    catch (...)
    {
        LOG_ERROR_CLIENT << "Crash";
    }
    LOG_SHUTDOWN;
    WSACleanup();

    return 0;
}
