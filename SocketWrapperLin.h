//
// Created by Elad Eliav on 2019-03-14.
//
#pragma once
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>

class SocketWrapper
{
private:
    sockaddr_in _address;
    int _socket;

public:
    std::string ip = "127.0.0.1";
    // empty, client constructors
    SocketWrapper();
    SocketWrapper(const std::string& ip, const int& port);

    void send(const std::string& data);
    std::string recv();
    void close();

    //server constructor
    SocketWrapper(const int& port, const int& maxCon);
    SocketWrapper(const sockaddr_in& address, const int& sock);
    SocketWrapper accept();

};
