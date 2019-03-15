//
// Created by elade on 3/15/2019.
//

#include "UniSocketSet.h"

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
    fd_set copy = this->_set;
    int fd = static_cast<int>(copy.fd_array[index]);
    UniSocket us(fd);
    return us;
}

void UniSocketSet::removeSock(const UniSocket &sock)
{
    FD_CLR(sock._sock._socket, &this->_set);
}

int UniSocketSet::select()
{
    fd_set copy = this->_set;
    return ::select(0, &copy, nullptr, nullptr, nullptr);
}