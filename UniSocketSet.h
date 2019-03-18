//
// Created by elade on 3/15/2019.
//

#include "UniSocket.h"
#include <array>
using std::array;

#if _WIN32
#include "fd_setWin.h"
#else
#include "FDSetWrapperLin.h"
#endif

#pragma once

class UniSocketSet
{
private:
    FDSetWrapper _set;
public:

    UniSocketSet();
    UniSocketSet(const UniSocket& masterSock);

    UniSocketSet(const UniSocket& masterSock, const int& timeout);

    void clearSet();

    void addSock(const UniSocket &sock);

    void removeSock(const UniSocket &sock);

    UniSocket sockAt(const int &index);

    vector<UniSocket> getReadySockets();

    template <class T, size_t N>
    bool objectInArray(const T& obj, const array<T, N>& a)
    {
        for(const T& i : a )
            if(i == obj)
                return true;
        return false;
    }

#if _WIN32
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
#else
#endif
};
