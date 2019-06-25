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

    int send(const void* data, int bufLen) const override;

    int raw_send(const void* data, int bufLen) const override;

    int recv(void* buf) const override;

    int raw_recv(void* buf, int bufLen) const override;

    int getSockId() override;

    void close() override;

    //server constructor
    SocketWrapper(int port, int maxCon);

    SocketWrapper(sockaddr_in address, int sock);

    explicit SocketWrapper(const int &sock);

    SocketWrapper accept();

    virtual bool valid();

private:
    bool initWinsock(WSAData& wsaData);
};

#endif //UNISOCKETS_TESTSOCKETWRAPPERWIN_H
