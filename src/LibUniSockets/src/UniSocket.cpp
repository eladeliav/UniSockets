//
// Created by elade on 3/14/2019.
//

#include "UniSockets/UniSocket.hpp"


UniSocket::UniSocket(std::string ip, int port, int timeout)
{
    _ip = ip;
    _timeout = timeout;
    _sock = SocketWrapper(_ip, port, _timeout);
}

int UniSocket::send(const void *data, int bufLen) const
{
    return _sock.send(data, bufLen);
}

int UniSocket::send(const std::string& data) const
{
    return send(data.c_str(), data.length());
}

int UniSocket::recv(void *buf) const
{
    return _sock.recv(buf);
}

int UniSocket::raw_recv(void *buf, int bufLen) const
{
    return this->_sock.raw_recv(buf, bufLen);
}

int UniSocket::raw_send(const void *data, int bufLen) const
{
    return this->_sock.raw_send(data, bufLen);
}

int UniSocket::raw_send(const std::string& data) const
{
    return raw_send(data.c_str(), data.length());
}

void UniSocket::close()
{
    _sock.close();
}

std::string UniSocket::getIp()
{
    return _ip;
}

UniSocket::UniSocket()
{}

UniSocket::UniSocket(const UniSocket &ref)
{
    _ip = ref._ip;
    _sock = ref._sock;
}

UniSocket::UniSocket(
        std::string ip,
        SocketWrapper sock)
{
    _ip = ip;
    _sock = sock;
}

UniSocket::UniSocket(int port, int maxCon, int timeout)
{
    _timeout = timeout;
    _sock = SocketWrapper(port, maxCon, timeout);
}

UniSocket::~UniSocket()
{
    //_sock.close();
}

UniSocket UniSocket::accept() const
{
    SocketWrapper sw;
    sw = _sock.accept();
    UniSocket us = UniSocket(sw.ip, sw);
    return us;
}

UniSocket::UniSocket(const SocketWrapper &ref)
{
    this->_sock = ref;
}

bool operator==(const UniSocket &lhs, const UniSocket &rhs)
{
    return lhs._sock.getSockId() == rhs._sock.getSockId();
}

bool operator!=(const UniSocket &lhs, const UniSocket &rhs)
{
    return !(lhs == rhs);
}

bool UniSocket::valid() const
{
    return this->_sock.valid();
}

int UniSocket::getSockId() const
{
    return this->_sock.getSockId();
}

void UniSocket::setTimeout(int timeout)
{
    this->_timeout = timeout;
    this->_sock.setTimeout(_timeout);
}

void UniSocket::cleanup()
{
#ifdef _WIN32
    WSACleanup();
#endif
}

UniSocket UniSocket::acceptIntervals() const
{
    SocketWrapper sw;
    sw = _sock.acceptIntervals();
    UniSocket us = UniSocket(sw.ip, sw);
    return us;
}

void UniSocket::broadcastToSet(std::string msg, std::vector<UniSocket> socks, bool raw, UniSocket ignoreSock)
{
    for (UniSocket &outSock : socks)
    {
        if (ignoreSock.valid() && outSock == ignoreSock)
            continue;
        if (raw)
            outSock.raw_send(msg.c_str(), msg.length());
        else
            outSock.send(msg.c_str(), msg.length());
    }
}
