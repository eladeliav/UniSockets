//
// Created by elade on 3/14/2019.
//

//TODOS HERE
//TODO: consider making << >> operator like a stream
//TODO: consider making separte class for UniServerSocket

#pragma once

#include <string>
#include <map>
#include <vector>
#include <ostream>
#include <sstream>

#include "UniSocketException.hpp"
#include "UniSockets/SocketWrappers/SocketWrapperUni.hpp"

//#if _WIN32
//#include "UniSockets/SocketWrappers/SocketWrapperWin.hpp"
//#else
//#include "SocketWrappers/SocketWrapperLin.hpp"
//#endif

class UniSocket;

class UniSocket
{
private:
    SocketWrapper _sock;
    std::string _ip;
    int _timeout = 0;
public:
    UniSocket(std::string ip, int port, int timeout=0); // connect socket
    UniSocket(int port, int maxCon, int timeout=0); //server socket
    UniSocket(std::string ip, SocketWrapper sock);

    UniSocket(); //empty socket
    UniSocket(const SocketWrapper &ref);

    UniSocket(const UniSocket &ref); //copy constructor
    ~UniSocket();

    std::string getIp(); //get ip of socket

    // send, receive, close
    int send(const void* data, int bufLen) const;

    int send(const std::string& data) const;

    int raw_send(const void* data, int bufLen) const;

    int raw_send(const std::string& data) const;

    int recv(void* buf) const;

    int raw_recv(void* buf, int bufLen) const;

    void close();

    int getSockId() const;

    UniSocket accept() const;

    UniSocket acceptIntervals() const;

    static void broadcastToSet(std::string msg, std::vector<UniSocket> socks, bool raw=true, UniSocket ignoreSock = UniSocket());

    void setTimeout(int timeout);

    bool valid() const;

    static void cleanup();

    friend bool operator==(const UniSocket &lhs, const UniSocket &rhs);

    friend bool operator!=(const UniSocket &lhs, const UniSocket &rhs);

    friend class UniSocketSet;
    friend class FDSetWrapper;
};

