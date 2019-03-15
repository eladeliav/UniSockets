//
// Created by elade on 3/14/2019.
//
#pragma once

#include <string>

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>
#include <ws2tcpip.h>

class SocketWrapper
{
private:
    SOCKADDR_IN _address;
    int _socket;

    bool initWinsock(WSADATA &wsaData);

public:
    std::string ip = "127.0.0.1";

    // empty, client constructors
    SocketWrapper();

    SocketWrapper(const std::string &ip, const int &port);

    void send(const std::string &data);

    std::string recv();

    void close();

    //server constructor
    SocketWrapper(const int &port, const int &maxCon);

    SocketWrapper(const SOCKADDR_IN &address, const int &sock);

    SocketWrapper accept();

};
