//
// Created by elade on 5/27/2019.
//
#pragma once
#ifndef UNISOCKETS_TESTSOCKETWRAPPERWIN_H
#define UNISOCKETS_TESTSOCKETWRAPPERWIN_H

#include "ISocketWrapper.hpp"

class SocketWrapper: public ISocketWrapper
{
public:
    SocketWrapper();

    SocketWrapper(std::string ip, int port);

    virtual int send(const void* data, int bufLen) const;

    virtual int recv(void* buf) const;

    virtual void close();

    //server constructor
    SocketWrapper(int port, int maxCon);

    SocketWrapper(sockaddr_in address, int sock);

    SocketWrapper(const int &sock);

    SocketWrapper accept();

    virtual bool valid();

private:
    bool initWinsock(WSAData& wsaData);
};

#endif //UNISOCKETS_TESTSOCKETWRAPPERWIN_H
