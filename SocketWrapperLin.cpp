//
// Created by Elad Eliav on 2019-03SOCKET_ERROR4.
//

#include "SocketWrapperLin.h"
#include "UniSocket.h"
#include <iostream>

using std::string;

#define SOCKET_ERROR -1

SocketWrapper::SocketWrapper()
{}

SocketWrapper::SocketWrapper(const string &ip, const int &port)
{
    _empty = false;
    this->ip = ip;
    this->_socket = ::socket(AF_INET, SOCK_STREAM, 0);
    if (this->_socket == SOCKET_ERROR)
        throw UniSocketException(UniSocketException::SOCKET_INIT);

    this->_address.sin_family = AF_INET;
    this->_address.sin_addr.s_addr = ::inet_addr(ip.c_str());
    this->_address.sin_port = htons(port);

    int result = connect(_socket, (struct sockaddr *) &_address, sizeof(_address));
    if (result == SOCKET_ERROR)
        throw UniSocketException(UniSocketException::CONNECT);
}

void SocketWrapper::send(const string &data)
{
    string msg = std::to_string(data.length()) + SIZE_HEADER_SPLITTER + data;
    int result = ::send(this->_socket, msg.c_str(), (int) msg.length(), 0);
    if (result == SOCKET_ERROR)
        throw UniSocketException(UniSocketException::SEND);
}

string SocketWrapper::recv()
{
    static bool headerReaded = false;
    static string result;
    static int datasize;
    int len;

    if (!headerReaded)
    {
        // header search
        do
        {
            char tmp = '\0';
            len = static_cast<int>(::recv(_socket, &tmp, 1, 0));
            if (len > 0)
            {
                result += tmp;
            }else if(len == 0)
            {
                throw UniSocketException(UniSocketException::DISCONNECTED);
            }
            else
            {
                throw UniSocketException(UniSocketException::TIMED_OUT);
            }
        } while (result.find(SIZE_HEADER_SPLITTER) == string::npos);
        headerReaded = true;
        int datastart = result.find(SIZE_HEADER_SPLITTER);
        datasize = std::stoi(result.substr(0, datastart).c_str());
        result.clear();
    }
    char data[datasize + 1];
    data[datasize] = '\0';
    ::recv(_socket, data, datasize, 0);
    headerReaded = false;
    return data;
}

void SocketWrapper::close()
{
    ::close(this->_socket);
}

SocketWrapper::SocketWrapper(const int &port, const int &maxCon)
{
    _empty = false;
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

SocketWrapper::SocketWrapper(const sockaddr_in &address, const int &sock)
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
    if (conn == SOCKET_ERROR)
        throw UniSocketException(UniSocketException::ACCEPT);

    SocketWrapper newClient = SocketWrapper(this->_address, conn);
    return newClient;
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
