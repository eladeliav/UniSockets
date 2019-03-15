//
// Created by elade on 3/14/2019.
//

#include "UniSocket.h"
#include <iostream>

UniSocketException::UniSocketException(std::string errorMsg)
{
    this->_errorMsg = errorMsg;
}

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

void UniSocket::send(const std::string &data)
{
    _sock.send(data);
}

void UniSocket::send(const std::string &data, int& result)
{
    _sock.send(data, result);
}

std::string UniSocket::recv()
{
    return _sock.recv();
}

std::string UniSocket::recv(int& result)
{
    return _sock.recv(result);
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

UniSocket UniSocket::accept(void)
{
    SocketWrapper sw = _sock.accept();
    UniSocket us = UniSocket(sw.ip, sw);
    return us;
}

UniSocket UniSocket::accept(int& result)
{
    SocketWrapper sw = _sock.accept(result);
    UniSocket us = UniSocket(sw.ip, sw);
    return us;
}

std::ostream &operator<<(std::ostream &os, const UniSocketException &uniSockExcept)
{
    os << uniSockExcept._errorMsg << std::endl;
    return os;
}

UniSocket::UniSocket(const SocketWrapper &ref)
{
    this->_sock = ref;
}

bool operator==(const UniSocket& lhs, const UniSocket& rhs)
{
    return lhs._sock._socket == rhs._sock._socket;
}

bool operator!=(const UniSocket& lhs, const UniSocket& rhs)
{
    return !(lhs==rhs);
}


bool UniSocket::valid()
{
    return this->_sock.valid();
}