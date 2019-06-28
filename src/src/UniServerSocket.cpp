//
// Created by elade on 6/28/2019.
//

#include "UniSockets/UniServerSocket.hpp"

// server constructor
UniServerSocket::UniServerSocket(int port, int maxCon, int timeout)
{
    _timeout = timeout;
#ifdef _WIN32
    // init Winsock
    WSADATA wsaData;
    if (!initWinsock(wsaData))
        throw UniSocketException(UniSocketException::WINSOCK);
#endif

    // init the server socket
    _socket = (SOCKET) ::socket(AF_INET, SOCK_STREAM, 0);
    if (_socket == (SOCKET) SOCKET_ERROR)
        throw UniSocketException(UniSocketException::SOCKET_INIT);

    // setting socket to be reusable (this is optional but it's good practice)
    int on = 1;
    int rc = setsockopt((SOCKET) _socket, SOL_SOCKET, SO_REUSEADDR, (char *) &on, sizeof(on));
    if (rc < 0)
        throw UniSocketException(UniSocketException::SOCKET_INIT);

    this->_address.sin_family = AF_INET;
    this->_address.sin_addr.s_addr = ::inet_addr(_ip.c_str());
    this->_address.sin_port = htons((u_short) port);

    if (_timeout > 0)
        setTimeout(_timeout);

    // bind socket to the given port and max connections
    rc = ::bind((SOCKET) _socket, (const sockaddr *) &_address, sizeof(_address));
    if (rc == SOCKET_ERROR)
        throw UniSocketException(UniSocketException::BIND);

    // being listening with the given maxConnections variable
    rc = ::listen((SOCKET) _socket, maxCon);
    if (rc == SOCKET_ERROR)
        throw UniSocketException(UniSocketException::LISTEN);
    _empty = false;
}
// helper function

#ifdef _WIN32
// init winsock
bool UniServerSocket::initWinsock(WSADATA &wsaData)
{
    //initializing winsock
    int iResult = ::WSAStartup(MAKEWORD(2, 2), &wsaData);
    return iResult == 0;
}
#endif

// returns if this socket is valid or not
bool UniServerSocket::valid() const
{
    return !_empty && _socket != (SOCKET) SOCKET_ERROR;
}

// socket operations

void UniServerSocket::setTimeout(int timeout)
{
    _timeout = timeout;
}

// accepts and returns a new socket
UniSocket UniServerSocket::accept() const
{
    int len = sizeof(struct sockaddr_in);

    int conn = (int) ::accept((SOCKET) this->_socket, (sockaddr*)&this->_address,
                              reinterpret_cast<socklen_t *>(&len));

    if (conn == SOCKET_ERROR || UNISOCK_ERRNO == UNISOCK_TIMEDOUT)
        throw UniSocketException(UniSocketException::ACCEPT);

    UniSocket newClient = UniSocket(this->_address, conn);
    return newClient;
}

// accepts sockets at intervals of the given timeout
UniSocket UniServerSocket::acceptIntervals() const
{
    fd_set set;
    struct timeval tv;
    tv.tv_sec = _timeout;
    tv.tv_usec = 0;

    FD_ZERO(&set);
    FD_SET(_socket, &set);

    int rv = 0;
    if(_timeout > DEFAULT_TIMEOUT)
        rv = select((SOCKET) _socket + 1, &set, nullptr, nullptr, &tv);
    else
        rv = select((SOCKET) _socket + 1, &set, nullptr, nullptr, nullptr);
    if (rv == -1)
        throw UniSocketException(UniSocketException::POLL);
    else if (rv == 0)
        throw UniSocketException(UniSocketException::TIMED_OUT);
    return accept();
}

void UniServerSocket::close()
{
#ifndef _WIN32
    if (::close((SOCKET)this->_socket) == SOCKET_ERROR)
        throw UniSocketException(UniSocketException::SOCKET_CLOSE);
#else
    if (::closesocket((SOCKET) this->_socket) == SOCKET_ERROR)
        throw UniSocketException(UniSocketException::SOCKET_CLOSE);
#endif
}
