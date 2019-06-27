//
// Created by elade on 5/27/2019.
//

#ifndef UNISOCKETS_ISOCKETWRAPPER_H
#define UNISOCKETS_ISOCKETWRAPPER_H

#if _WIN32
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <mstcpip.h>
#define sockaddr_in SOCKADDR_IN
#define errno WSAGetLastError()
#define ETIMEDOUT WSAETIMEDOUT
#define ECONNRESET WSAECONNRESET
#define EAGAIN WSAEGAIN
#define EWOULDBLOCK WSAEWOULDBLOCK
#else

#include <arpa/inet.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <errno.h>
#define SOCKET int
#define SOCKET_ERROR -1
#endif

// includes for both
#include <string>
#include <vector>
#define SIZE_HEADER_SPLITTER ':'

class ISocketWrapper
{
public:
    std::string ip = "127.0.0.1";
public:
    virtual int send(const void *data, int bufLen) const = 0;

    virtual int raw_send(const void *data, int bufLen) const = 0;

    virtual int recv(void *buf) const = 0;

    virtual int raw_recv(void *buf, int bufLen) const = 0;

    virtual void close() = 0;

    virtual bool valid() = 0;

    virtual int getSockId() const = 0;

    virtual void setTimeout(int timeout) = 0;

protected:
    sockaddr_in _address;
    SOCKET _socket;
    bool _empty = true;
    int _timeout = 0;
};

#endif //UNISOCKETS_ISOCKETWRAPPER_H
