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

    virtual bool valid() override;

private:
    bool initWinsock(WSAData& wsaData);
    int _timeout;
};

#endif //UNISOCKETS_TESTSOCKETWRAPPERWIN_H
