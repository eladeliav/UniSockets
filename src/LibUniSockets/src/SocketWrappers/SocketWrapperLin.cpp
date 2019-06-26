//
// Created by Elad Eliav on 2019-03SOCKET_ERROR4.
//
#include "UniSockets/UniSocket.hpp"
#include <iostream>
#include <UniSockets/SocketWrappers/SocketWrapperLin.hpp>


using std::string;

#define SOCKET_ERROR -1

SocketWrapper::SocketWrapper()
{}

SocketWrapper::SocketWrapper(string ip, int port, int timeout)
{
    _empty = false;
    this->ip = ip;
    _timeout = timeout;
    this->_socket = ::socket(AF_INET, SOCK_STREAM, 0);
    if (this->_socket == SOCKET_ERROR)
        throw UniSocketException(UniSocketException::SOCKET_INIT);

    this->_address.sin_family = AF_INET;
    this->_address.sin_addr.s_addr = ::inet_addr(ip.c_str());
    this->_address.sin_port = htons(port);

    setTimeout(_timeout);

    int result = connect(_socket, (struct sockaddr *) &_address, sizeof(_address));
    if (result == SOCKET_ERROR)
        throw UniSocketException(UniSocketException::CONNECT);
}

int numDigits(int number)
{
    int digits = 0;
    if (number < 0) digits = 1;
    while(number)
    {
        number /= 10;
        digits++;
    }
    return digits;
}


int SocketWrapper::raw_send(const void *data, int bufLen) const
{
    char *pBuf = (char *) data;
    int result = ::send(this->_socket, pBuf, bufLen, 0);
    if (errno == ETIMEDOUT)
    {
        throw UniSocketException(UniSocketException::TIMED_OUT);
    }
    if (result == SOCKET_ERROR)
        throw UniSocketException(UniSocketException::SEND);
    return result;
}

int SocketWrapper::raw_recv(void *buf, int bufLen) const
{
    int bytesReceived = ::recv(this->_socket, static_cast<char *>(buf), bufLen, 0);
    if (errno == ETIMEDOUT || errno == EAGAIN || errno == EWOULDBLOCK)
    {
        throw UniSocketException(UniSocketException::TIMED_OUT);
    }
    if (errno == ECONNRESET || bytesReceived == 0)
    {
        throw UniSocketException(UniSocketException::DISCONNECTED);
    }
    return bytesReceived;
}

int SocketWrapper::send(const void* data, int bufLen) const
{
    char *pBuf = (char *) data;
    string msg = std::to_string(bufLen) + SIZE_HEADER_SPLITTER + pBuf;
    int size = numDigits(bufLen) + sizeof(SIZE_HEADER_SPLITTER) + bufLen;
    int result = ::send(this->_socket, msg.c_str(), size, 0);
    if (errno == ETIMEDOUT)
    {
        throw UniSocketException(UniSocketException::TIMED_OUT);
    }
    if (result == SOCKET_ERROR)
        throw UniSocketException(UniSocketException::SEND);
    return result;
}

int SocketWrapper::recv(void* buf) const
{
    int size = 0;
    int bytesReceived = 0;
    char sizeBuf[1] = {'\0'};
    string sizeString;

    do
    {
        bytesReceived = ::recv(this->_socket, sizeBuf, 1, 0);
        if (bytesReceived > 0)
        {
            sizeString += *sizeBuf;
        }
        else if (errno == ECONNRESET || bytesReceived == 0)
        {
            throw UniSocketException(UniSocketException::DISCONNECTED);
        }
        else if (errno == ETIMEDOUT)
        {
            throw UniSocketException(UniSocketException::TIMED_OUT);
        }
    } while (*sizeBuf != SIZE_HEADER_SPLITTER || *sizeBuf == (char) string::npos);

    int sizeHeaderIndex = static_cast<int>(sizeString.find(SIZE_HEADER_SPLITTER));
    try
    {
        size = std::stoi(sizeString.substr(0, sizeHeaderIndex));
    } catch (std::invalid_argument &e)
    {
        std::cout << "invalid data size" << std::endl;
        return -1;
    }
    ::recv(this->_socket, static_cast<char *>(buf), size, 0);
    return size;
}

void SocketWrapper::close()
{
    if(::close(this->_socket) == SOCKET_ERROR)
        throw (UniSocketException::SOCKET_CLOSE);
}

SocketWrapper::SocketWrapper(int port, int maxCon, int timeout)
{
    _empty = false;
    _timeout = timeout;
    this->_socket = ::socket(AF_INET, SOCK_STREAM, 0);
    if (this->_socket == SOCKET_ERROR)
        throw UniSocketException(UniSocketException::SOCKET_INIT);

    int on = 1;
    int rc = setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR,
                        (char *) &on, sizeof(on));
    if (rc < 0)
        throw UniSocketException(UniSocketException::SOCKET_INIT);

    this->_address.sin_family = AF_INET;
    this->_address.sin_addr.s_addr = ::inet_addr(ip.c_str());
    this->_address.sin_port = htons((u_short) port);

    setTimeout(_timeout);

    int result = ::bind(this->_socket, reinterpret_cast<const sockaddr *>(&this->_address), sizeof(_address));
    if (result == SOCKET_ERROR)
    {
        throw UniSocketException(UniSocketException::BIND);
    }

    result = ::listen(this->_socket, maxCon);
    if (result == SOCKET_ERROR)
        throw UniSocketException(UniSocketException::LISTEN);
}

string extractIp(sockaddr_in &address)
{
    char buff[18];
    sprintf(buff,
            "%d.%d.%d.%d",
            int(address.sin_addr.s_addr & 0xFF),
            int((address.sin_addr.s_addr & 0xFF00) >> 8),
            int((address.sin_addr.s_addr & 0xFF0000) >> 16),
            int((address.sin_addr.s_addr & 0xFF000000) >> 24)
    );
    return buff;
}

SocketWrapper::SocketWrapper(sockaddr_in address, int sock)
{
    _empty = false;
    this->_socket = sock;
    this->_address = address;
    ip = extractIp(_address);
}

SocketWrapper SocketWrapper::accept()
{
    int len = sizeof(struct sockaddr_in);

    int conn = (int) ::accept(this->_socket, reinterpret_cast<sockaddr *>(&this->_address),
                              reinterpret_cast<socklen_t *>(&len));
    if (conn == SOCKET_ERROR || errno == ETIMEDOUT)
        throw UniSocketException(UniSocketException::ACCEPT);

    SocketWrapper newClient = SocketWrapper(this->_address, conn);
    newClient.setTimeout(_timeout);
    return newClient;
}

SocketWrapper SocketWrapper::acceptIntervals()
{
    fd_set set;
    struct timeval tv;
    tv.tv_sec = _timeout;
    tv.tv_usec = 0;
    FD_ZERO(&set);
    FD_SET(_socket, &set);

    int rv = select(_socket+1, &set, nullptr, nullptr, &tv);
    if(rv == -1)
        throw UniSocketException(UniSocketException::POLL);
    else if(rv == 0)
        throw UniSocketException(UniSocketException::TIMED_OUT);
    return accept();
}


SocketWrapper::SocketWrapper(const int &sock)
{
    _empty = false;
    this->_socket = sock;
}

bool SocketWrapper::valid()
{
    return !_empty && this->_socket > -1;
}

void SocketWrapper::setTimeout(int timeout)
{
    _timeout = timeout;
    struct timeval tv;
    tv.tv_sec = _timeout;
    tv.tv_usec = 0;
    setsockopt(this->_socket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
}

int SocketWrapper::getSockId() const
{
    return this->_socket;
}