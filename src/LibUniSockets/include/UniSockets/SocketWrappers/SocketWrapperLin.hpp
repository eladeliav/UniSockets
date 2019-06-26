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

    SocketWrapper(std::string ip, int port, int timeout);

    int send(const void* data, int bufLen) const override;

    int raw_send(const void* data, int bufLen) const override;

    int recv(void* buf) const override;

    int raw_recv(void* buf, int bufLen) const override;

    int getSockId() const override;

    void close() override;

    void setTimeout(int timeout) override;

    //server constructor
    SocketWrapper(int port, int maxCon, int timeout);

    SocketWrapper(sockaddr_in address, int sock);

    explicit SocketWrapper(const int &sock);

    SocketWrapper accept();

    SocketWrapper acceptIntervals();

    virtual bool valid() override;

private:
    int _timeout;

};

