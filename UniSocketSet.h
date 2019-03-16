//
// Created by elade on 3/15/2019.
//

#include "UniSocket.h"
#include <array>
using std::array;

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

    void addSock(const UniSocket &sock);

    void removeSock(const UniSocket &sock);

    UniSocket sockAt(const int &index);

    int select();

    template <size_t N>
    void broadcast(const std::string &msg, const array<UniSocket, N>& ignoreSocks)
};
