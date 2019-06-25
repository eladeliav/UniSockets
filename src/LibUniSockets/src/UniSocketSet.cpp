//
// Created by elade on 3/15/2019.
//

#include "UniSockets/UniSocketSet.hpp"
#include <iostream>

void UniSocketSet::clearSet()
{
    this->_set.clearSet();
}

void UniSocketSet::addSock(const UniSocket &sock)
{
    this->_set.addSock(sock);
}

UniSocket UniSocketSet::sockAt(const int &index)
{
    return this->_set.sockAt(index);
}

void UniSocketSet::removeSock(const UniSocket &sock)
{
    this->_set.removeSock(sock);
}

vector<UniSocket> UniSocketSet::getReadySockets()
{
    return this->_set.getReadySockets();
}

UniSocketSet::UniSocketSet(const UniSocket &masterSock)
{
    _set = FDSetWrapper(masterSock);
    _timeout = masterSock._timeout;
}

UniSocketSet::UniSocketSet()
{

}

vector<UniSocket> UniSocketSet::getAllFDS()
{
    return _set.getAllFDS();
}
