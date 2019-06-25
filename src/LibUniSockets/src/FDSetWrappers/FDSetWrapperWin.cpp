//
// Created by Elad Eliav on 2019-03-18.
//

#include "UniSockets/FDSetWrappers/FDSetWrapperWin.hpp"
#include "UniSockets/UniSocketSet.hpp"

FDSetWrapper::FDSetWrapper()
{
    clearSet();
}

FDSetWrapper::FDSetWrapper(const UniSocket& masterSock)
{
    clearSet();
    addSock(masterSock);
    _timeout = masterSock._timeout;
    _masterFd = masterSock.getSockId();
}

void FDSetWrapper::clearSet()
{
    FD_ZERO(&this->_master);
}

void FDSetWrapper::addSock(const UniSocket &sock)
{
    FD_SET((SOCKET)sock._sock._socket, &this->_master);
}

UniSocket FDSetWrapper::sockAt(const int &index)
{
    int fd = static_cast<int>(_copy.fd_array[index]);
    UniSocket us(fd);
    return us;
}

void FDSetWrapper::removeSock(const UniSocket &sock)
{
    FD_CLR((SOCKET)sock._sock._socket, &this->_master);
    _copy = _master;
}

int FDSetWrapper::select()
{
    timeval tv;
    tv.tv_sec = _timeout;
    tv.tv_usec = 0;
    _copy = _master;
    return ::select(_masterFd+1, &_copy, nullptr, nullptr, &tv);
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
    for(int i = 0; i < (int)_master.fd_count;i++)
    {
        readySocks.emplace_back(UniSocket(static_cast<const int &>(_master.fd_array[i])));
    }
    return readySocks;
}
