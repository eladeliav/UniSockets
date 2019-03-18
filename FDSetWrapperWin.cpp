//
// Created by Elad Eliav on 2019-03-18.
//

#include "FDSetWrapperWin.h"
#include "UniSocketSet.h"

FDSetWrapper::FDSetWrapper()
{
    clearSet();
}

FDSetWrapper::FDSetWrapper(const UniSocket& masterSock)
{
    clearSet();
    addSock(masterSock);
}

FDSetWrapper::FDSetWrapper(const UniSocket &masterSock, const int &timeout)
{
    clearSet();
    addSock(masterSock);
}

void FDSetWrapper::clearSet()
{
    FD_ZERO(&this->_master);
}

void FDSetWrapper::addSock(const UniSocket &sock)
{
    FD_SET(sock._sock._socket, &this->_master);
}

UniSocket FDSetWrapper::sockAt(const int &index)
{
    int fd = static_cast<int>(_copy.fd_array[index]);
    UniSocket us(fd);
    return us;
}

void FDSetWrapper::removeSock(const UniSocket &sock)
{
    FD_CLR(sock._sock._socket, &this->_master);
    _copy = _master;
}

int FDSetWrapper::select()
{
    _copy = _master;
    return ::select(0, &_copy, nullptr, nullptr, nullptr);
}

vector<UniSocket> FDSetWrapper::getReadySockets()
{
    vector<UniSocket> readySocks;
    int socketCount = select();
    for (int i = 0; i < socketCount; i++)
    {
        UniSocket currentSock = UniSocket(sockAt(i));
        readySocks.emplace_back(currentSock);
    }
    return readySocks;
}

vector<UniSocket> FDSetWrapper::getAllFDS()
{
    vector<UniSocket> readySocks;
    for(int i = 0; i < _master.fd_count;i++)
    {
        readySocks.emplace_back(UniSocket(static_cast<const int &>(_master.fd_array[i])));
    }
    return readySocks;
}
