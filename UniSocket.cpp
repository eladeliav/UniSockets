//
// Created by elade on 3/14/2019.
//

#include "UniSocket.h"
#include <iostream>


UniSocket::UniSocket(
        const std::string &ip,
        const int &port
)
{
    _ip = ip;
    _sock = SocketWrapper(_ip, port);
}

UniSocket::UniSocket(const int &fd)
{
    _sock = SocketWrapper(fd);
}

int UniSocket::send(const std::string &data)
{
    try
    {
        _sock.send(data);
    }
    catch (UniSocketException &e)
    {
        std::cout << e << std::endl;
        return e.getError();
    }
    return 0;
}

UniSocketStruct<std::string> UniSocket::recv()
{
    std::string receivedString;
    try
    {
        receivedString = _sock.recv();
    }
    catch (UniSocketException &e)
    {
        std::cout << e << std::endl;
        return UniSocketStruct<std::string>("", e.getError());
    }
    return UniSocketStruct<std::string>(receivedString, 1);
}

void UniSocket::close(void)
{
    _sock.close();
}

std::string UniSocket::getIp(void)
{
    return _ip;
}

// fuer Server nuetzlich -----------------

UniSocket::UniSocket(void)
{}

UniSocket::UniSocket(const UniSocket &ref)
{
    _ip = ref._ip;
    _sock = ref._sock;
}

UniSocket::UniSocket(
        const std::string &ip,
        const SocketWrapper &sock)
{
    _ip = ip;
    _sock = sock;
}

UniSocket::UniSocket(
        const int &port,
        const int &queueSize
)
{
    _sock = SocketWrapper(port, queueSize);
}

UniSocket::~UniSocket(void)
{
    //_sock.close();
}

UniSocketStruct<UniSocket> UniSocket::accept(void)
{
    SocketWrapper sw;
    try
    {
        sw = _sock.accept();
    }
    catch (UniSocketException &e)
    {
        std::cout << e << std::endl;
        return UniSocketStruct(UniSocket(), e.getError());
    }
    UniSocket us = UniSocket(sw.ip, sw);
    return UniSocketStruct(us, 1);
}

UniSocket::UniSocket(const SocketWrapper &ref)
{
    this->_sock = ref;
}

bool operator==(const UniSocket &lhs, const UniSocket &rhs)
{
    return lhs._sock._socket == rhs._sock._socket;
}

bool operator!=(const UniSocket &lhs, const UniSocket &rhs)
{
    return !(lhs == rhs);
}

bool UniSocket::valid()
{
    return this->_sock.valid();
}