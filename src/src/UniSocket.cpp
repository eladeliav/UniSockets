//
// Created by elad on 6/27/19.
//

#include "UniSockets/UniSocket.hpp"
#include <iostream>

#ifndef _WIN32
#include <cstring>
#endif

// constructors
//client constructor
UniSocket::UniSocket(std::string ip, int port, int timeout)
{
    // init ip and timeout variables
    this->ip = ip;
    _timeout = timeout;
#ifdef _WIN32
    // init Winsock
    WSADATA wsaData;
    if (!initWinsock(wsaData))
        throw UniSocketException(UniSocketException::WINSOCK);
#endif

    // initialize the socket
    _socket = (SOCKET) ::socket(AF_INET, SOCK_STREAM, 0);
    if (_socket == (SOCKET) SOCKET_ERROR)
        throw UniSocketException(UniSocketException::SOCKET_INIT);

    // initialize the address info var
    _address.sin_family = AF_INET;
    _address.sin_addr.s_addr = ::inet_addr(ip.c_str());
    _address.sin_port = htons(port);

    if (_timeout > DEFAULT_TIMEOUT)
        setTimeout(_timeout);

    int connResult = ::connect((SOCKET) _socket, (struct sockaddr *) &_address, sizeof(_address));
    if (connResult == SOCKET_ERROR)
        throw UniSocketException(UniSocketException::CONNECT);

    _empty = false;
}

// constructor from existing fd and address
UniSocket::UniSocket(sockaddr_in address, int sock)
{
    this->_empty = false;
    this->_socket = sock;
    this->_address = address;
    ip = UniSocket::extractIp(_address);
}

// helper functions

#ifdef _WIN32

// init winsock
bool UniSocket::initWinsock(WSADATA &wsaData)
{
    //initializing winsock
    int iResult = ::WSAStartup(MAKEWORD(2, 2), &wsaData);
    return iResult == 0;
}

#endif

// returns number of digits in a given number
int UniSocket::numDigits(int num)
{
    int digits = 0;
    if (num < 0) digits = 1;
    while (num)
    {
        num /= 10;
        digits++;
    }
    return digits;
}

// extracts a stringified ip from a sockaddr_in
std::string UniSocket::extractIp(sockaddr_in &address)
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

// returns if this socket is valid or not
bool UniSocket::valid() const
{
    return !_empty && _socket != (SOCKET) SOCKET_ERROR;
}

// returns fd #
int UniSocket::getSockId() const
{
    return (int) _socket;
}

// socket operations

// sends with size header
int UniSocket::send(const void *data, int bufLen) const
{
    char *pBuf = (char *) data;
    std::string msg = std::to_string(bufLen) + SIZE_HEADER_SPLITTER + pBuf;
    int size = numDigits(bufLen) + sizeof(SIZE_HEADER_SPLITTER) + bufLen;
    int result = ::send(this->_socket, msg.c_str(), size, 0);
    if (UNISOCK_ERRNO == UNISOCK_TIMEDOUT)
    {
        throw UniSocketException(UniSocketException::TIMED_OUT);
    }
    if (result == SOCKET_ERROR)
        throw UniSocketException(UniSocketException::SEND);
    return result;
}

int UniSocket::send(const std::string &data) const
{
    return send(data.c_str(), data.length());
}

// sends raw
int UniSocket::raw_send(const void *data, int bufLen) const
{
    char *pBuf = (char *) data;
    int result = ::send(this->_socket, pBuf, bufLen, 0);
    if (UNISOCK_ERRNO == UNISOCK_TIMEDOUT)
    {
        throw UniSocketException(UniSocketException::TIMED_OUT);
    }
    if (result == SOCKET_ERROR)
        throw UniSocketException(UniSocketException::SEND);
    return result;
}

int UniSocket::raw_send(const std::string &data) const
{
    return raw_send(data.c_str(), data.length());
}

// receives with size header
int UniSocket::recv(void *buf) const
{
    int size = 0;
    int bytesReceived = 0;
    char sizeBuf[1] = {'\0'};
    std::string sizeString;

    do
    {
        memset(sizeBuf, 0, sizeof(sizeBuf));
        bytesReceived = ::recv(this->_socket, sizeBuf, 1, 0);
        if (bytesReceived > 0)
        {
            sizeString += *sizeBuf;
        } else if (UNISOCK_ERRNO == UNISOCK_CONNRESET || bytesReceived == 0)
        {
            throw UniSocketException(UniSocketException::DISCONNECTED);
        } else if (UNISOCK_ERRNO == UNISOCK_TIMEDOUT || UNISOCK_ERRNO == UNISOCK_WOULDBLOCK)
        {
            throw UniSocketException(UniSocketException::TIMED_OUT);
        }
    } while (*sizeBuf != SIZE_HEADER_SPLITTER || *sizeBuf == (char) std::string::npos);

    int sizeHeaderIndex = static_cast<int>(sizeString.find(SIZE_HEADER_SPLITTER));
    try
    {
        size = std::stoi(sizeString.substr(0, sizeHeaderIndex));
    } catch (std::invalid_argument &e)
    {
        std::cout << "Bad Size buf: " << sizeString << std::endl;
        throw UniSocketException(UniSocketException::BAD_SIZE_HEADER);
    }
    ::recv(this->_socket, static_cast<char *>(buf), size, 0);
    return size;
}

// receives raw
int UniSocket::raw_recv(void *buf, int bufLen) const
{
    int bytesReceived = ::recv(this->_socket, static_cast<char *>(buf), bufLen, 0);
    if (UNISOCK_ERRNO == UNISOCK_TIMEDOUT || UNISOCK_ERRNO == UNISOCK_WOULDBLOCK)
    {
        throw UniSocketException(UniSocketException::TIMED_OUT);
    }
    if (UNISOCK_ERRNO == UNISOCK_CONNRESET || bytesReceived == 0)
    {
        throw UniSocketException(UniSocketException::DISCONNECTED);
    }
    return bytesReceived;
}

// sets socket timeout for send and recv operations ( setting it is different for win32 vs linux
void UniSocket::setTimeout(int timeout)
{
#ifdef _WIN32
    _timeout = timeout;
    DWORD dTimeout = timeout * 1000;
    setsockopt((SOCKET)
                       this->_socket, SOL_SOCKET, SO_RCVTIMEO, (const char *) &dTimeout, sizeof dTimeout);
#else
    _timeout = timeout;
    struct timeval tv;
    tv.tv_sec = _timeout;
    tv.tv_usec = 0;
    setsockopt(this->_socket, SOL_SOCKET, SO_RCVTIMEO, (const char *) &tv, sizeof(tv));
#endif
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

std::vector<UniSocket> UniSocket::select(std::vector<UniSocket> &socks, int timeout)
{
    fd_set set;
    struct timeval tv;
    if (timeout > DEFAULT_TIMEOUT)
        tv.tv_sec = timeout;
    else
        tv.tv_sec = 0;
    tv.tv_usec = 0;

    FD_ZERO(&set);
    int biggest = socks.at(0)._socket;
    for (auto const &s : socks)
    {
        FD_SET(s._socket, &set);
        if (biggest < (int) s._socket)
            biggest = s._socket;
    }

    std::vector<UniSocket> readable;
    int rv = 0;
    rv = ::select((biggest + 1), &set, nullptr, nullptr, &tv);
    if (rv)
    {
        for (size_t i = 0; i < socks.size() && rv; i++)
        {
            if(FD_ISSET(socks.at(i)._socket, &set))
            {
                readable.push_back(socks.at(i));
                rv--;
            }
        }
    }
    return readable;
}


void UniSocket::close()
{
#ifndef _WIN32
    if (::close((SOCKET)this->_socket) == SOCKET_ERROR)
        throw UniSocketException(UniSocketException::SOCKET_CLOSE);
#else
    if (::closesocket((SOCKET) this->_socket) == SOCKET_ERROR)
        throw UniSocketException(UniSocketException::SOCKET_CLOSE);
#endif
}

void UniSocket::cleanup()
{
#ifdef _WIN32
    WSACleanup();
#endif
}