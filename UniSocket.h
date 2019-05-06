//
// Created by elade on 3/14/2019.
//

#pragma once

#include <string>
#include <map>
using std::map;
#include "UniSocketException.h"

#if _WIN32
#include "SocketWrapperWin.h"
#else
#include "SocketWrapperLin.h"
#endif

#define SIZE_HEADER_SPLITTER ':'
//TODO: fix glitch in linux where someone leaves and the server stops responding
//TODO: make send and recv return buf instead of std::string
//TODO: don't catch exceptions

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

    int recv(void* buf);

    void close();

    UniSocket accept(void);

    bool valid();

    friend bool operator==(const UniSocket &lhs, const UniSocket &rhs);

    friend bool operator!=(const UniSocket &lhs, const UniSocket &rhs);

    friend class UniSocketSet;
    friend class FDSetWrapper;
};

