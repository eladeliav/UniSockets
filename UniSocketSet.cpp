//
// Created by elade on 3/15/2019.
//

#include "UniSocketSet.h"

#ifndef _WIN32
#define fd_array fds_bits
#define fd_count fds_bits
#endif

UniSocketSet::UniSocketSet()
{
    clearSet();
}

UniSocketSet::~UniSocketSet()
{
    clearSet();
}

void UniSocketSet::clearSet()
{
    FD_ZERO(&this->_set);
}

void UniSocketSet::addSock(const UniSocket &sock)
{
    FD_SET(sock._sock._socket, &this->_set);
}

UniSocket UniSocketSet::sockAt(const int &index)
{
    int fd = static_cast<int>(_copy.fd_array[index]);
    UniSocket us(fd);
    return us;
}

void UniSocketSet::removeSock(const UniSocket &sock)
{
    FD_CLR(sock._sock._socket, &this->_set);
    _copy = _set;
}

int UniSocketSet::select()
{
    _copy = _set;
    return ::select(0, &_copy, nullptr, nullptr, nullptr);
}

template <class T>
bool objectInVector(const T& obj, const vector<T>& vec)
{
    for(auto const& value: vec)
    {
        if(obj == value)
            return true;
    }
    return false;
}

template <class T, size_t N>
bool objectInArray(const T& obj, const array<T, N>& a)
{
    for(const T& i : a )
        if(i == obj)
            return true;
    return false;
}

#if WIN32

template <size_t N>
void UniSocketSet::broadcast(const std::string &msg, const array<UniSocket, N>& ignoreSocks)
{
    for (size_t i = 0; i < _set.fd_count; i++)
    {
        UniSocket outSock(static_cast<const int &>(_set.fd_array[i]));
        if (!objectInArray(outSock, ignoreSocks))
        {
            outSock.send(msg);
        }
    }
}


#else
void UniSocketSet::broadcast(const std::string& msg, const UniSocket& ignoreSock)
{
    int max_sd = _set.fds_bits[0];
    fd_set working_set;
    memcpy(&working_set, &this->_set, sizeof(this->_set));

    int rc = ::select(max_sd+1, &working_set, nullptr, nullptr, nullptr);
    if(rc > 0)
    {
        for(int i = 0; i <= max_sd;++i)
        {
            if(FD_ISSET(i, &working_set) && i != ignoreSock._sock._socket)
            {
                UniSocket outSock(i);
                outSock.send(msg);
            }
        }
    }
}
#endif