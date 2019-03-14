//
// Created by elade on 3/14/2019.
//
#include "UniSocket.h"
#include "SocketWrapperWin.h"
#include <ws2tcpip.h>
#include <iostream>

using namespace std;

SocketWrapper::SocketWrapper()
{

}

SocketWrapper::SocketWrapper(const string &ip, const int &port)
{
    WSADATA wsaData;
    if (!initWinsock(wsaData))
        throw UniSocketException("Failed to initialize winsock");

    this->ip = ip;
    this->_socket = (int) ::socket(AF_INET, SOCK_STREAM, 0);
    if (this->_socket == INVALID_SOCKET)
        throw UniSocketException("Failed to initialize socket");

    this->_address.sin_family = AF_INET;
    this->_address.sin_addr.s_addr = ::inet_addr(ip.c_str());
    this->_address.sin_port = htons(port);

    int result = ::connect((SOCKET) this->_socket, (SOCKADDR *) &this->_address, sizeof(SOCKADDR_IN));
    if (result == SOCKET_ERROR)
        throw UniSocketException("Failed to connect to " + ip);
}

bool SocketWrapper::initWinsock(WSADATA &wsaData)
{
    //initializing winsock
    int iResult = ::WSAStartup(MAKEWORD(2, 2), &wsaData);
    return iResult == 0;
}

void SocketWrapper::send(const string &data)
{
    string msg = to_string(data.length()) + SIZE_HEADER_SPLITTER + data;
    ::send((SOCKET) this->_socket, msg.c_str(), (int) msg.length(), 0);
}

string SocketWrapper::recv()
{
    static bool readHeader = false;
    static string result;
    static int dataSize = 0;
    int bytesReceived = 0;
    char tmpBuf = '\0';
    if(!readHeader)
    {
        do
        {
            bytesReceived = ::recv((SOCKET)this->_socket, &tmpBuf, 1, 0);
            if(bytesReceived > 0)
            {
                result += tmpBuf;
            } else
            {
                throw UniSocketException("Timed out");
            }
        } while(result.find(SIZE_HEADER_SPLITTER) == string::npos);
        int startIndex = (int)result.find(SIZE_HEADER_SPLITTER);
        dataSize = stoi(result.substr(0, static_cast<unsigned long long int>(startIndex)));
        result.clear();
        readHeader = true;
    }
    char* dataBuf = new char[dataSize + 1];
    ::recv((SOCKET)this->_socket, dataBuf, dataSize, MSG_WAITALL);
    readHeader = false;
    return dataBuf;
}

void SocketWrapper::close()
{
    WSACleanup();
    closesocket((SOCKET)this->_socket);
}

//server
SocketWrapper::SocketWrapper(const int& port, const int& maxCon)
{
    WSADATA wsaData;
    if (!initWinsock(wsaData))
        throw UniSocketException("Failed to initialize winsock");

    this->_socket = (int) ::socket(AF_INET, SOCK_STREAM, 0);
    if(this->_socket == INVALID_SOCKET)
        throw UniSocketException("Failed to initialize socket");

    this->_address.sin_family = AF_INET;
    this->_address.sin_addr.s_addr = ::inet_addr(ip.c_str());
    this->_address.sin_port = htons((u_short) port);

    int result = ::bind((SOCKET)this->_socket, (SOCKADDR*)&this->_address, sizeof(SOCKADDR_IN));
    if(result == SOCKET_ERROR)
    {
        cout << WSAGetLastError();
        throw UniSocketException("Failed to bind socket to port ");
    }


    result = ::listen((SOCKET)this->_socket, maxCon);
    if(result == SOCKET_ERROR)
        throw UniSocketException("Failed to listen on port " + port);
}

SocketWrapper::SocketWrapper(const SOCKADDR_IN& address, const int& sock)
{
    this->_socket = sock;
    this->_address = address;

    char buff[18];
    sprintf(buff,
            "%d.%d.%d.%d",
            int(_address.sin_addr.s_addr&0xFF),
            int((_address.sin_addr.s_addr&0xFF00)>>8),
            int((_address.sin_addr.s_addr&0xFF0000)>>16),
            int((_address.sin_addr.s_addr&0xFF000000)>>24)
    );
    ip = buff;
}

SocketWrapper SocketWrapper::accept()
{
    int len = sizeof(struct sockaddr_in);

    int conn = (int)::accept((SOCKET)this->_socket, (SOCKADDR*)&this->_address, &len);
    if(conn == INVALID_SOCKET)
        throw UniSocketException("Failed to accept new client");

    SocketWrapper newClient = SocketWrapper(this->_address, conn);
    return newClient;
}
