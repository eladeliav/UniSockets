//
// Created by elade on 3/15/2019.
//

#include "UniSocket.h"

#pragma once

class UniSocketSet
{
private:
    fd_set _set;
    fd_set _copy;
public:

    UniSocketSet();
    ~UniSocketSet();

    void clearSet();
    void addSock(const UniSocket& sock);
    void removeSock(const UniSocket& sock);
    UniSocket sockAt(const int& index);

    int select();

    void broadcast(const std::string& msg, const UniSocket& ignoreSock);

};
