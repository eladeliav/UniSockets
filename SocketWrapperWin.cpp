//
// Created by elade on 3/14/2019.
//
#include "UniSocket.h"
#include "SocketWrapperWin.h"
#include <ws2tcpip.h>
#include <iostream>

using std::string;

SocketWrapper::SocketWrapper()
{

}

SocketWrapper::SocketWrapper(const string &ip, const int &port)
{
    _empty = false;
    WSADATA wsaData;
    if (!initWinsock(wsaData))
        throw UniSocketException(UniSocketException::WINSOCK);

    this->ip = ip;
    this->_socket = (int) ::socket(AF_INET, SOCK_STREAM, 0);
    if (this->_socket == INVALID_SOCKET)
        throw UniSocketException(UniSocketException::SOCKET_INIT);

    this->_address.sin_family = AF_INET;
    this->_address.sin_addr.s_addr = ::inet_addr(ip.c_str());
    this->_address.sin_port = htons(port);

    int result = ::connect((SOCKET)
    this->_socket, (SOCKADDR * ) & this->_address, sizeof(SOCKADDR_IN));
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

void SocketWrapper::send(const string &data)
{
    string msg = std::to_string(data.length()) + SIZE_HEADER_SPLITTER + data;
    int result = ::send((SOCKET)this->_socket, msg.c_str(), (int) msg.length(), 0);
    if(result == SOCKET_ERROR)
        throw UniSocketException(UniSocketException::SEND);
}

std::string SocketWrapper::recv()
{
    int size = 0;
    int bytesReceived = 0;
    char sizeBuf[1];
    string sizeString;

    ZeroMemory(&sizeBuf, 1);
    do
    {
        bytesReceived = ::recv((SOCKET)this->_socket, sizeBuf, 1, 0);
        if(bytesReceived > 0)
        {
            sizeString += sizeBuf;
        }
        else if(bytesReceived == 0)
        {
            throw UniSocketException(UniSocketException::DISCONNECTED);
        }else
        {
            throw UniSocketException(UniSocketException::TIMED_OUT);
        }
    }while(sizeString.find(SIZE_HEADER_SPLITTER) == string::npos);

    int sizeHeaderIndex = static_cast<int>(sizeString.find(SIZE_HEADER_SPLITTER));
    size = std::stoi(sizeString.substr(0, sizeHeaderIndex));
    int sizeSave = size; //because ZeroMemory also zeroes out the int for some reason
    char buf[size];
    ZeroMemory(&sizeBuf, size);
    ::recv((SOCKET)this->_socket, buf, sizeSave, 0);
    return buf;
}

void SocketWrapper::close()
{
    WSACleanup();
    closesocket((SOCKET)
    this->_socket);
}

//server
SocketWrapper::SocketWrapper(const int &port, const int &maxCon)
{
    _empty = false;
    WSADATA wsaData;
    if (!initWinsock(wsaData))
        throw UniSocketException(UniSocketException::WINSOCK);

    this->_socket = (int) ::socket(AF_INET, SOCK_STREAM, 0);
    if (this->_socket == INVALID_SOCKET)
        throw UniSocketException(UniSocketException::SOCKET_INIT);

    this->_address.sin_family = AF_INET;
    this->_address.sin_addr.s_addr = ::inet_addr(ip.c_str());
    this->_address.sin_port = htons((u_short) port);

    int result = ::bind((SOCKET)
    this->_socket, (SOCKADDR * ) & this->_address, sizeof(SOCKADDR_IN));
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

SocketWrapper::SocketWrapper(const SOCKADDR_IN &address, const int &sock)
{
    _empty=false;
    this->_socket = sock;
    this->_address = address;
    ip = extractIp(_address);
}

SocketWrapper SocketWrapper::accept()
{
    int conn = static_cast<int>(::accept((SOCKET)this->_socket, nullptr, nullptr));
    if (conn == INVALID_SOCKET)
        throw UniSocketException(UniSocketException::ACCEPT);

    SocketWrapper newClient = SocketWrapper(this->_address, conn);
    return newClient;
}

SocketWrapper::SocketWrapper(const int &sock)
{
    _empty=false;
    this->_socket = sock;
}

bool SocketWrapper::valid()
{
    return !_empty && this->_socket != INVALID_SOCKET;
}