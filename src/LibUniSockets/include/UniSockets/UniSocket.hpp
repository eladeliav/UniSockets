//
// Created by elade on 3/14/2019.
//

#pragma once

#include <string>
#include <map>
using std::map;
#include "UniSocketException.hpp"

#if _WIN32
#include "UniSockets/SocketWrappers/SocketWrapperWin.hpp"
#else
#include "SocketWrappers/SocketWrapperLin.hpp"
#endif

#define SIZE_HEADER_SPLITTER ':'

class UniSocket;

class UniSocket
{
private:
    SocketWrapper _sock;
    std::string _ip;
public:
    UniSocket(const std::string &ip, const int &port); // connect socket
    UniSocket(const int &port, const int &maxCon); //server socket
    UniSocket(const std::string &ip, const SocketWrapper &sock);

    UniSocket(const int &fd);

    UniSocket(); //empty socket
    UniSocket(const SocketWrapper &ref);

    UniSocket(const UniSocket &ref); //copy constructor
    ~UniSocket();

    std::string getIp(); //get ip of socket

    // send, receive, close
    int send(const void* data, int bufLen);

    int raw_send(const void* data, int bufLen);

    int recv(void* buf);

    int raw_recv(void* buf, int bufLen);

    void close();

    int getSockId();

    UniSocket accept(void);

    bool valid();

    friend bool operator==(const UniSocket &lhs, const UniSocket &rhs);

    friend bool operator!=(const UniSocket &lhs, const UniSocket &rhs);

    friend class UniSocketSet;
    friend class FDSetWrapper;
};

