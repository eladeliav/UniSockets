//
// Created by elade on 6/28/2019.
//

#ifndef UNISOCKETS_UNISERVERSOCKET_HPP
#define UNISOCKETS_UNISERVERSOCKET_HPP

#include "UniSocket.hpp"

class UniServerSocket
{
private:
    int _timeout = DEFAULT_TIMEOUT;
    sockaddr_in _address;
    SOCKET _socket;
    bool _empty = true;
    std::string _ip = DEFAULT_IP;
public:
    UniServerSocket() {}; // empty socket
    //server constructors
    UniServerSocket(int port, int maxCon, int timeout = 0); // constructor for a server

    // server functions
    UniSocket accept() const; // accept a socket (hogs thread)

    UniSocket acceptIntervals() const; // accepts at intervals according to this->_timeout

    void setTimeout(int timeout); // set timeout for send/recv

    bool valid() const; // checks if socket is valid

    void close(); // close sock

private:
#ifdef _WIN32
    bool initWinsock(WSAData& wsaData);
#endif
};
#endif //UNISOCKETS_UNISERVERSOCKET_HPP
