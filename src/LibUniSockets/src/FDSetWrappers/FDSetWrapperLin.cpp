//
// Created by Elad Eliav on 2019-03-18.
//

#include "UniSockets/FDSetWrappers/FDSetWrapperLin.hpp"
#include <cstring>

FDSetWrapper::FDSetWrapper(const UniSocket &masterSock)
{
    clearSet();
    this->_timeout = masterSock._timeout;
    _masterSock = masterSock;
    for(auto& cl : allClients)
        cl = UniSocket(0);
}

void FDSetWrapper::addSock(const UniSocket &sock)
{
    FD_SET(sock.getSockId(), &this->_master);
}

void FDSetWrapper::clearSet()
{
    FD_ZERO(&this->_master);
}

void FDSetWrapper::removeSock(const UniSocket &sock)
{
    FD_CLR(sock.getSockId(), &this->_master);
    _copy = _master;
}

UniSocket FDSetWrapper::sockAt(const int &index)
{
    return UniSocket(fds[index].fd);
}

int FDSetWrapper::select()
{
    timeval tv;
    tv.tv_sec = _timeout;
    tv.tv_usec = 0;
    _copy = _master;
    return ::select(_masterSock.getSockId()+1, &_copy, nullptr, nullptr, &tv);
}

vector<UniSocket> FDSetWrapper::getReadySockets()
{
    std::vector<UniSocket> readySocks;
    clearSet();
    addSock(_masterSock);
    int max_sd = _masterSock.getSockId();

    //adding valid clients to set
    for(auto& cl : allClients)
    {
        if(cl.getSockId() > 0)
            FD_SET(cl.getSockId(), &_master);
        if(cl.getSockId() > max_sd)
            max_sd = cl.getSockId();
    }
    int activity = select();

    if(activity < 0 && errno != EINTR)
    {
        throw(UniSocketException::POLL);
    }

    if(FD_ISSET(_masterSock.getSockId(), &_master))
    {
        readySocks.push_back(_masterSock);
        allClients.push_back()
    }


}

FDSetWrapper::FDSetWrapper()
{
    clearSet();
}

vector<UniSocket> FDSetWrapper::getAllFDS()
{
    vector<UniSocket> allSocks;
    current_size = nfds;
    allSocks.reserve(static_cast<unsigned long>(current_size));
    for(int i = 0; i < current_size;i++)
    {
        allSocks.push_back(fds[i].fd);
    }
    return allSocks;
}

int FDSetWrapper::select()
{
    timeval tv;
    tv.tv_sec = _timeout;
    tv.tv_usec = 0;
    _copy = _master;
    return ::select(_masterFd+1, &_copy, nullptr, nullptr, &tv);
}
