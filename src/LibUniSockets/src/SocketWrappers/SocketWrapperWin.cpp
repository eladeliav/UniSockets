//
// Created by elade on 3/14/2019.
//
#include "UniSockets/UniSocket.hpp"
#include <iostream>
#include <UniSockets/SocketWrappers/SocketWrapperWin.hpp>

using std::string;

SocketWrapper::SocketWrapper()
{

}

SocketWrapper::SocketWrapper(string ip, int port, int timeout)
{
    _empty = false;
    _timeout = timeout;
    WSADATA wsaData;
    if (!initWinsock(wsaData))
        throw UniSocketException(UniSocketException::WINSOCK);

    this->ip = ip;
    this->_socket = (int) ::socket(AF_INET, SOCK_STREAM, 0);
    if (this->_socket == (int) INVALID_SOCKET)
        throw UniSocketException(UniSocketException::SOCKET_INIT);

    this->_address.sin_family = AF_INET;
    this->_address.sin_addr.s_addr = ::inet_addr(ip.c_str());
    this->_address.sin_port = htons(port);

    setTimeout(_timeout);

    int result = ::connect((SOCKET)
                                   this->_socket, (SOCKADDR *) &this->_address, sizeof(SOCKADDR_IN));
    if (result == SOCKET_ERROR)
    {
        throw UniSocketException(UniSocketException::CONNECT);
    }
}

bool SocketWrapper::initWinsock(WSADATA &wsaData)
{
    //initializing winsock
    int iResult = ::WSAStartup(MAKEWORD(2, 2), &wsaData);
    return iResult == 0;
}

template<class T>
int numDigits(T number)
{
    int digits = 0;
    if (number < 0) digits = 1; // remove this line if '-' counts as a digit
    while (number)
    {
        number /= 10;
        digits++;
    }
    return digits;
}

int SocketWrapper::raw_send(const void *data, int bufLen) const
{
    char *pBuf = (char *) data;
    int result = ::send((SOCKET) this->_socket, pBuf, bufLen, 0);
    if (WSAGetLastError() == WSAETIMEDOUT)
    {
        throw UniSocketException(UniSocketException::TIMED_OUT);
    }
    if (result == SOCKET_ERROR)
        throw UniSocketException(UniSocketException::SEND);
    return result;
}

int SocketWrapper::raw_recv(void *buf, int bufLen) const
{
    int bytesReceived = ::recv((SOCKET) this->_socket, static_cast<char *>(buf), bufLen, 0);
    if (WSAGetLastError() == WSAETIMEDOUT)
    {
        throw UniSocketException(UniSocketException::TIMED_OUT);
    }
    if (bytesReceived == 0)
    {
        throw UniSocketException(UniSocketException::DISCONNECTED);
    }
    return bytesReceived;
}

int SocketWrapper::send(const void *data, int bufLen) const
{
    char *pBuf = (char *) data;
    string msg = std::to_string(bufLen) + SIZE_HEADER_SPLITTER + pBuf;
    int size = numDigits(bufLen) + sizeof(SIZE_HEADER_SPLITTER) + bufLen;
    int result = ::send((SOCKET) this->_socket, msg.c_str(), size, 0);
    if (WSAGetLastError() == WSAETIMEDOUT)
    {
        throw UniSocketException(UniSocketException::TIMED_OUT);
    }
    if (result == SOCKET_ERROR)
        throw UniSocketException(UniSocketException::SEND);
    return result;
}

int SocketWrapper::recv(void *buf) const
{
    int size = 0;
    int bytesReceived = 0;
    char sizeBuf[1] = {'\0'};
    string sizeString;

    do
    {
        bytesReceived = ::recv((SOCKET) this->_socket, sizeBuf, 1, 0);
        if (bytesReceived > 0)
        {
            sizeString += *sizeBuf;
        } else if (bytesReceived == 0)
        {
            throw UniSocketException(UniSocketException::DISCONNECTED);
        }
        if (WSAGetLastError() == WSAETIMEDOUT)
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
    ::recv((SOCKET) this->_socket, static_cast<char *>(buf), size, 0);
    return size;
}

void SocketWrapper::close()
{
    if(closesocket((SOCKET)this->_socket) == SOCKET_ERROR)
        throw (UniSocketException::SOCKET_CLOSE);
}

//server
SocketWrapper::SocketWrapper(int port, int maxCon, int timeout)
{
    _empty = false;
    _timeout = timeout;
    WSADATA wsaData;
    if (!initWinsock(wsaData))
        throw UniSocketException(UniSocketException::WINSOCK);

    this->_socket = (int) ::socket(AF_INET, SOCK_STREAM, 0);
    if ((int) INVALID_SOCKET == this->_socket)
        throw UniSocketException(UniSocketException::SOCKET_INIT);

    this->_address.sin_family = AF_INET;
    this->_address.sin_addr.s_addr = ::inet_addr(ip.c_str());
    this->_address.sin_port = htons((u_short) port);

    setTimeout(_timeout);

    int result = ::bind((SOCKET)
                                this->_socket, (SOCKADDR *) &this->_address, sizeof(SOCKADDR_IN));
    if (result == SOCKET_ERROR)
    {
        throw UniSocketException(UniSocketException::BIND);
    }

    result = ::listen((SOCKET)
                              this->_socket, maxCon);
    if (result == SOCKET_ERROR)
    {
        throw UniSocketException(UniSocketException::LISTEN);
    }
}

string extractIp(SOCKADDR_IN &address)
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
    sockaddr_in newAddressInfo;
    int addrsize = sizeof(newAddressInfo);
    int conn = static_cast<int>(::accept((SOCKET) this->_socket, (struct sockaddr *) &newAddressInfo, &addrsize));
    if (conn == (int) INVALID_SOCKET || WSAGetLastError() == WSAETIMEDOUT)
        throw UniSocketException(UniSocketException::ACCEPT);

    SocketWrapper newClient = SocketWrapper(newAddressInfo, conn);
    return newClient;
}

SocketWrapper::SocketWrapper(const int &sock)
{
    _empty = false;
    this->_socket = sock;
}

bool SocketWrapper::valid()
{
    return !_empty && this->_socket != (int) INVALID_SOCKET;
}

int SocketWrapper::getSockId() const
{
    return this->_socket;
}

void SocketWrapper::setTimeout(int timeout)
{
    DWORD dTimeout = timeout * 1000;
    setsockopt((SOCKET) this->_socket, SOL_SOCKET, SO_RCVTIMEO, (const char *) &dTimeout, sizeof dTimeout);
}