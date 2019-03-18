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

    template <class T, size_t N>
    bool objectInArray(const T& obj, const array<T, N>& a)
    {
        for(const T& i : a )
            if(i == obj)
                return true;
        return false;
    }

    template <class T, size_t N>
    void broadcast(const std::string& msg, const std::array<T, N> &ignoreSocks)
    {
        for (size_t i = 0; i < _set.fd_count; i++)
        {
            UniSocket outSock((int)_set.fd_array[i]);
            if (!objectInArray(outSock, ignoreSocks))
            {
                outSock.send(msg);
            }
        }
    }
};
