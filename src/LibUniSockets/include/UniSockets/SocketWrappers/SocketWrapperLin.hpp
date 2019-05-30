//
// Created by Elad Eliav on 2019-03-14.
//
#pragma once

#include "ISocketWrapper.hpp"

class SocketWrapper: public ISocketWrapper
{
public:
    // empty, client constructors
    SocketWrapper();

    SocketWrapper(std::string ip, int port);

    virtual int send(const void* data, int bufLen) const;

    int raw_send(const void* data, int bufLen) const override;

    virtual int recv(void* buf) const;

    int raw_recv(void* buf, int bufLen) const override;

    virtual void close();

    //server constructor
    SocketWrapper(int port, int maxCon);

    SocketWrapper(sockaddr_in address, int sock);

    SocketWrapper(const int &sock);

    SocketWrapper accept();

    virtual bool valid();

};

