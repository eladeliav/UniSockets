//
// Created by Elad Eliav on 2019-03-14.
//
#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/select.h>
#include <string>

class SocketWrapper
{
private:
    sockaddr_in _address;
    bool _empty = true;
public:
    int _socket;
    std::string ip = "127.0.0.1";

    // empty, client constructors
    SocketWrapper();

    SocketWrapper(const std::string &ip, const int &port);

    void send(const std::string &data);

    void send(const std::string &data, int &result);

    std::string recv();

    std::string recv(int &r);

    void close();

    //server constructor
    SocketWrapper(const int &port, const int &maxCon);

    SocketWrapper(const int &sock);

    SocketWrapper(const sockaddr_in &address, const int &sock);

    SocketWrapper accept();

    SocketWrapper accept(int &result);

    bool valid();

};
