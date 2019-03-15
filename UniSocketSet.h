//
// Created by elade on 3/15/2019.
//

#include "UniSocket.h"

#pragma once

class UniSocketSet
{
private:
    fd_set _set;
public:

    UniSocketSet();
    ~UniSocketSet();

    void clearSet();
    void addSock(const UniSocket& sock);
    void removeSock(const UniSocket& sock);
    UniSocket sockAt(const int& index);

    int select();

};
