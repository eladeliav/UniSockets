//
// Created by elade on 3/14/2019.
//

#ifndef UNISOCKETS_HPP
#define UNISOCKETS_HPP

#if _WIN32
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <mstcpip.h>
#define UNISOCK_ERRNO WSAGetLastError()
#define UNISOCK_TIMEDOUT WSAETIMEDOUT
#define UNISOCK_CONNRESET WSAECONNRESET
#define UNISOCK_WOULDBLOCK WSAEWOULDBLOCK
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
#define UNISOCKERRNO errno
#define UNISOCK_TIMEDOUT ETIMEDOUT
#define UNISOCK_CONNRESET ECONNRESET
#define UNISOCK_WOULDBLOCK EWOULDBLOCK
#endif

// includes for both
#include <string>
#include <vector>
#define SIZE_HEADER_SPLITTER ':'

#include "UniSockets/UniSocketException.hpp"


#define DEFAULT_BUFFER_LEN 2048

class UniSocket;

class UniSocket
{
public:
    std::string ip = "127.0.0.1";
private:
    sockaddr_in _address;
    SOCKET _socket;
    bool _empty = true;
    int _timeout = 0;
public:
    UniSocket() = default; // empty constructor

    UniSocket(std::string ip, int port, int timeout); // client constructor

    int send(const void* data, int bufLen) const; // send with size header

    int send(const std::string& data) const;

    int raw_send(const void* data, int bufLen) const; // send data raw (as is)

    int raw_send(const std::string& data) const;

    int recv(void* buf) const; // receive data with size header

    int raw_recv(void* buf, int bufLen) const; // receive data raw (as is)

    int getSockId() const; // return fd

    void close(); // close sock

    void setTimeout(int timeout); // set timeout for send/recv

    //server constructors
    UniSocket(int port, int maxCon, int timeout); // constructor for a server

    UniSocket(sockaddr_in address, int sock); //  init a socket wrapper through address info and fd #

    // server functions
    UniSocket accept() const; // accept a socket (hogs thread)

    UniSocket acceptIntervals() const; // accepts at intervals according to this->_timeout

    virtual bool valid() const; // checks if socket is valid

    // cleanup for winsock

    static void cleanup();

    //operator overloads
    bool operator==(const UniSocket &rhs)
    {
        return this->getSockId() == rhs.getSockId();
    }

    bool operator!=(const UniSocket &rhs)
    {
        return !(*this == rhs);
    }

    template<class T>
    UniSocket& operator<<(T t)
    {
        raw_send(t);
        return *this;
    }

    UniSocket& operator>>(void* buf)
    {
        raw_recv(buf, DEFAULT_BUFFER_LEN);
        return *this;
    }

private:
    static int numDigits(int num);
    static std::string extractIp(sockaddr_in &address);
#ifdef _WIN32
    bool initWinsock(WSAData& wsaData);
#endif
};

#endif //UNISOCKETS_HPP
