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
        throw UniSocketException("Failed to initialize socket");

    this->_address.sin_family = AF_INET;
    this->_address.sin_addr.s_addr = ::inet_addr(ip.c_str());
    this->_address.sin_port = htons(port);

    int result = connect(_socket, (struct sockaddr *) &_address, sizeof(_address));
    if (result == SOCKET_ERROR)
        throw UniSocketException("Failed to connect to ip");
}

void SocketWrapper::send(const string &data)
{
    string msg = std::to_string(data.length()) + SIZE_HEADER_SPLITTER + data;
    ::send(this->_socket, msg.c_str(), msg.length(), 0);
}

void SocketWrapper::send(const string &data, int& result)
{
    string msg = std::to_string(data.length()) + SIZE_HEADER_SPLITTER + data;
    result = ::send(this->_socket, msg.c_str(), msg.length(), 0);
}

string SocketWrapper::recv()
{
    static bool readHeader = false;
    static string result;
    static int dataSize = 0;
    int bytesReceived = 0;
    char tmpBuf = '\0';
    if (!readHeader)
    {
        do
        {
            bytesReceived = static_cast<int>(::recv(this->_socket, &tmpBuf, 1, 0));
            if (bytesReceived > 0)
            {
                result += tmpBuf;
            } else
            {
                throw UniSocketException("Timed out");
            }
        } while (result.find(SIZE_HEADER_SPLITTER) == string::npos);
        int startIndex = static_cast<int>(result.find(SIZE_HEADER_SPLITTER));
        dataSize = stoi(result.substr(0, static_cast<unsigned long long int>(startIndex)));
        result.clear();
        readHeader = true;
    }
    char *dataBuf = new char[dataSize + 1];
    ::recv(this->_socket, dataBuf, static_cast<size_t>(dataSize), MSG_WAITALL);
    readHeader = false;
    return dataBuf;
}

std::string recv(int& r)
{
    r = 0;
    static bool readHeader = false;
    static string result;
    static int dataSize = 0;
    int bytesReceived = 0;
    char tmpBuf = '\0';
    if (!readHeader)
    {
        do
        {
            bytesReceived = static_cast<int>(::recv(this->_socket, &tmpBuf, 1, 0));
            if (bytesReceived > 0)
            {
                result += tmpBuf;
            } else
            {
                throw UniSocketException("Timed out");
            }
        } while (result.find(SIZE_HEADER_SPLITTER) == string::npos);
        int startIndex = static_cast<int>(result.find(SIZE_HEADER_SPLITTER));
        dataSize = stoi(result.substr(0, static_cast<unsigned long long int>(startIndex)));
        result.clear();
        readHeader = true;
        r = static_cast<int>(result.size());
    }
    char *dataBuf = new char[dataSize + 1];
    ::recv(this->_socket, dataBuf, static_cast<size_t>(dataSize), MSG_WAITALL);
    readHeader = false;
    return dataBuf;
}

void SocketWrapper::close()
{
    int result = ::close(this->_socket);
    if (result == -1)
    {
        std::cout << "Failed to close socket" << std::endl;
        throw UniSocketException("Failed to close socket properly");
    }

}

SocketWrapper::SocketWrapper(const int &port, const int &maxCon)
{
    _empty = false;
    this->_socket = (int) ::socket(AF_INET, SOCK_STREAM, 0);
    if (this->_socket == SOCKET_ERROR)
        throw UniSocketException("Failed to initialize socket");

    this->_address.sin_family = AF_INET;
    this->_address.sin_addr.s_addr = ::inet_addr(ip.c_str());
    this->_address.sin_port = htons((u_short) port);

    int result = ::bind(this->_socket, reinterpret_cast<const sockaddr *>(&this->_address), sizeof(_address));
    if (result == SOCKET_ERROR)
    {
        string message = "Failed to bind socket to port " + std::to_string(port);
        throw UniSocketException(message);
    }

    result = ::listen(this->_socket, maxCon);
    if (result == SOCKET_ERROR)
        throw UniSocketException("Failed to listen on port ");
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
        throw UniSocketException("Couldn't accept to socket");

    SocketWrapper newClient = SocketWrapper(this->_address, conn);
    return newClient;
}

SocketWrapper SocketWrapper::accept(int& result)
{
    int len = sizeof(struct sockaddr_in);

    int conn = (int) ::accept(this->_socket, reinterpret_cast<sockaddr *>(&this->_address),
                              reinterpret_cast<socklen_t *>(&len));
    result=conn;

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
    return !_empty && this->_socket != INVALID_SOCKET;
}
