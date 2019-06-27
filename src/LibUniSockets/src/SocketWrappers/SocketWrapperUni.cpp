//
// Created by elad on 6/27/19.
//

#include "UniSockets/SocketWrappers/SocketWrapperUni.hpp"

// constructors
//client constructor
SocketWrapper::SocketWrapper(std::string ip, int port, int timeout)
{
    // init ip and timeout variables
    this->ip = ip;
    _timeout = timeout;
#ifdef _WIN32
    // init Winsock
    WSADATA wsaData;
    if(!initWinsock(wsaData))
        throw UniSocketException(UniSocketException::WINSOCK);
#endif

    // initialize the socket
    _socket = (SOCKET) ::socket(AF_INET, SOCK_STREAM, 0);
    if (_socket == (SOCKET)SOCKET_ERROR)
        throw UniSocketException(UniSocketException::SOCKET_INIT);

    // initialize the address info var
    _address.sin_family = AF_INET;
    _address.sin_addr.s_addr = ::inet_addr(ip.c_str());
    _address.sin_port = htons(port);

    if (_timeout > 0)
        setTimeout(_timeout);

    int connResult = ::connect((SOCKET) _socket, (struct sockaddr *) &_address, sizeof(_address));
    if (connResult == SOCKET_ERROR)
        throw UniSocketException(UniSocketException::CONNECT);

    _empty = false;
}

// server constructor
SocketWrapper::SocketWrapper(int port, int maxCon, int timeout)
{
    _timeout = timeout;
#ifdef _WIN32
    // init Winsock
    WSADATA wsaData;
    if(!initWinsock(wsaData))
        throw UniSocketException(UniSocketException::WINSOCK);
#endif

    // init the server socket
    _socket = (SOCKET) ::socket(AF_INET, SOCK_STREAM, 0);
    if (_socket == (SOCKET)SOCKET_ERROR)
        throw UniSocketException(UniSocketException::SOCKET_INIT);

    // setting socket to be reusable (this is optional but it's good practice)
    int on = 1;
    int rc = setsockopt((SOCKET) _socket, SOL_SOCKET, SO_REUSEADDR, (char *) &on, sizeof(on));
    if (rc < 0)
        throw UniSocketException(UniSocketException::SOCKET_INIT);

    this->_address.sin_family = AF_INET;
    this->_address.sin_addr.s_addr = ::inet_addr(ip.c_str());
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

// constructor from existing fd and address
SocketWrapper::SocketWrapper(sockaddr_in address, int sock)
{
    this->_empty = false;
    this->_socket = sock;
    this->_address = address;
    ip = SocketWrapper::extractIp(_address);
}

// helper functions
// returns number of digits in a given number
int SocketWrapper::numDigits(int num)
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
std::string SocketWrapper::extractIp(sockaddr_in &address)
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
bool SocketWrapper::valid()
{
    return !_empty && _socket != (SOCKET)SOCKET_ERROR;
}

// returns fd #
int SocketWrapper::getSockId() const
{
    return (int) _socket;
}

#ifdef _WIN32
// init winsock
bool SocketWrapper::initWinsock(WSADATA &wsaData)
{
    //initializing winsock
    int iResult = ::WSAStartup(MAKEWORD(2, 2), &wsaData);
    return iResult == 0;
}
#endif

// socket operations

// sends with size header
int SocketWrapper::send(const void *data, int bufLen) const
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

// sends raw
int SocketWrapper::raw_send(const void *data, int bufLen) const
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

// receives with size header
int SocketWrapper::recv(void *buf) const
{
    int size = 0;
    int bytesReceived = 0;
    char sizeBuf[1] = {'\0'};
    std::string sizeString;

    do
    {
        bytesReceived = ::recv(this->_socket, sizeBuf, 1, 0);
        if (bytesReceived > 0)
        {
            sizeString += *sizeBuf;
        } else if (UNISOCK_ERRNO == UNISOCK_CONNRESET || bytesReceived == 0)
        {
            throw UniSocketException(UniSocketException::DISCONNECTED);
        } else if (UNISOCK_ERRNO == UNISOCK_TIMEDOUT)
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
        throw UniSocketException(UniSocketException::BAD_SIZE_HEADER);
    }
    ::recv(this->_socket, static_cast<char *>(buf), size, 0);
    return size;
}

// receives raw
int SocketWrapper::raw_recv(void *buf, int bufLen) const
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
void SocketWrapper::setTimeout(int timeout)
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
    setsockopt(this->_socket, SOL_SOCKET, SO_RCVTIMEO, (const char *) &tv, sizeof tv);
#endif
}

// accepts and returns a new socket
SocketWrapper SocketWrapper::accept()
{
    int len = sizeof(struct sockaddr_in);

    int conn = (int) ::accept((SOCKET)this->_socket, reinterpret_cast<sockaddr *>(&this->_address),
                              reinterpret_cast<socklen_t *>(&len));

    if (conn == SOCKET_ERROR || UNISOCK_ERRNO == UNISOCK_TIMEDOUT)
        throw UniSocketException(UniSocketException::ACCEPT);

    SocketWrapper newClient = SocketWrapper(this->_address, conn);
    return newClient;
}

// accepts sockets at intervals of the given timeout
SocketWrapper SocketWrapper::acceptIntervals()
{
    fd_set set;
    struct timeval tv;
    tv.tv_sec = _timeout;
    tv.tv_usec = 0;
    FD_ZERO(&set);
    FD_SET(_socket, &set);

    int rv = select((SOCKET)_socket + 1, &set, nullptr, nullptr, &tv);
    if (rv == -1)
        throw UniSocketException(UniSocketException::POLL);
    else if (rv == 0)
        throw UniSocketException(UniSocketException::TIMED_OUT);
    return accept();
}

void SocketWrapper::close()
{
#ifndef _WIN32
    if (::close((SOCKET)this->_socket) == SOCKET_ERROR)
        throw UniSocketException(UniSocketException::SOCKET_CLOSE);
#else
    if (::closesocket((SOCKET)this->_socket) == SOCKET_ERROR)
        throw UniSocketException(UniSocketException::SOCKET_CLOSE);
#endif
}


