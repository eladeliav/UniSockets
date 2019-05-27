//
// Created by elade on 5/27/2019.
//

#ifndef UNISOCKETS_ISOCKETWRAPPER_H
#define UNISOCKETS_ISOCKETWRAPPER_H
#include <string>
#if _WIN32
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <mstcpip.h>
#define sockaddr_in SOCKADDR_IN
#else
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#endif

class ISocketWrapper
{
public:
    int _socket;
    std::string ip = "127.0.0.1";

    virtual int send(const void* data, int bufLen) const = 0;

    virtual int raw_send(const void* data, int bufLen) const = 0;

    virtual int recv(void* buf) const = 0;

    virtual int raw_recv(void* buf) const = 0;

    virtual void close() = 0;

    virtual bool valid() = 0;

protected:
    sockaddr_in _address;

    bool _empty = true;
};

#endif //UNISOCKETS_ISOCKETWRAPPER_H
