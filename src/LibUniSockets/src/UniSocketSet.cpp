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
    try
    {
        return this->_set.getReadySockets();
    }
    catch(UniSocketException& e)
    {
        return vector<UniSocket>();
    }
}

UniSocketSet::UniSocketSet(const UniSocket &masterSock)
{
    _set = FDSetWrapper(masterSock);
}

UniSocketSet::UniSocketSet()
{

}

vector<UniSocket> UniSocketSet::getAllFDS()
{
    return _set.getAllFDS();
}
