//
// Created by elad on 6/27/19.
//

#ifndef UNISOCKETS_SOCKETWRAPPERUNI_HPP
#define UNISOCKETS_SOCKETWRAPPERUNI_HPP

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

//#include "UniSockets/SocketWrappers/ISocketWrapper.hpp"
#include "UniSockets/UniSocketException.hpp"

class SocketWrapper
{
public:
    std::string ip = "127.0.0.1";
private:
    sockaddr_in _address;
    SOCKET _socket;
    bool _empty = true;
    int _timeout = 0;
public:
    SocketWrapper() = default; // empty constructor

    SocketWrapper(std::string ip, int port, int timeout); // client constructor

    int send(const void* data, int bufLen) const; // send with size header

    int raw_send(const void* data, int bufLen) const; // send data raw (as is)

    int recv(void* buf) const; // receive data with size header

    int raw_recv(void* buf, int bufLen) const; // receive data raw (as is)

    int getSockId() const; // return fd

    void close(); // close sock

    void setTimeout(int timeout); // set timeout for send/recv

    //server constructors
    SocketWrapper(int port, int maxCon, int timeout); // constructor for a server

    SocketWrapper(sockaddr_in address, int sock); //  init a socket wrapper through address info and fd #

    // server functions
    SocketWrapper accept(); // accept a socket (hogs thread)

    SocketWrapper acceptIntervals(); // accepts at intervals according to this->_timeout

    virtual bool valid(); // checks if socket is valid
private:
    static int numDigits(int num);
    static std::string extractIp(sockaddr_in &address);
#ifdef _WIN32
    bool initWinsock(WSAData& wsaData);
#endif
};


#endif //UNISOCKETS_SOCKETWRAPPERUNI_HPP
