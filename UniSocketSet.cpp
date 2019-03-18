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