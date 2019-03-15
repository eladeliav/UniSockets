//
// Created by elade on 3/14/2019.
//

#pragma once
#include <string>
#if _WIN32
#include "SocketWrapperWin.h"
#else
#include "SocketWrapperLin.h"
#endif
#define SIZE_HEADER_SPLITTER ":\n"

class UniSocketException :public std::exception
{
private:
    std::string _errorMsg;
public:
    UniSocketException(std::string errorMsg);
    friend std::ostream& operator<<(std::ostream& os, const UniSocketException& uniSockExcept);
};

class UniSocket
{
private:
    SocketWrapper _sock;
    std::string _ip;
public:
    UniSocket(const std::string& ip, const int& port); // connect socket
    UniSocket(const int& port, const int& maxCon); //server socket
    UniSocket(const std::string & ip, const SocketWrapper & sock);
    UniSocket(); //empty socket
    UniSocket(const UniSocket& ref); //copy constructor
    ~UniSocket();
    std::string getIp(); //get ip of socket

    // send, receive, close
    void send(const std::string& data);
    std::string recv();
    void close();
    UniSocket accept(void);
};

