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
    bool initWinsock(WSADATA &wsaData);
    bool _empty = true;

public:
    int _socket;
    std::string ip = "127.0.0.1";

    // empty, client constructors
    SocketWrapper();

    SocketWrapper(const std::string &ip, const int &port);

    void send(const std::string &data);
    void send(const std::string& data, int& result);

    std::string recv();
    std::string recv(int& r);

    void close();

    //server constructor
    SocketWrapper(const int &port, const int &maxCon);

    SocketWrapper(const SOCKADDR_IN &address, const int &sock);
    SocketWrapper(const int& sock);

    SocketWrapper accept();

    SocketWrapper accept(int& result);

    bool valid();

};
