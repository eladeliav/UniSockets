//
// Created by Elad Eliav on 2019-03-18.
//

#include "FDSetWrapperLin.h"

FDSetWrapper::FDSetWrapper(const UniSocket &masterSock)
{
    clearSet();
    fds[0].fd = masterSock._sock._socket;
    fds[0].events = POLLIN;
}

FDSetWrapper::FDSetWrapper(const UniSocket &masterSock, const int &timeout)
{
    clearSet();
    this->timeout = timeout;
    fds[0].fd = masterSock._sock._socket;
    fds[0].events = POLLIN;
}

void FDSetWrapper::addSock(const UniSocket &sock)
{
    fds[nfds].fd = sock._sock._socket;
    fds[nfds].events = POLLIN;
    nfds++;
}

void FDSetWrapper::clearSet()
{
    memset(fds, 0, sizeof(fds));
}

void FDSetWrapper::removeSock(const UniSocket &sock)
{
    for(int i = 0; i < nfds;i++)
    {
        if(fds[i].fd == sock._sock._socket)
            fds[i].fd = -1;
    }

    //compress the set (squeeze together so -1 sockets are at the end
    for (int i = 0; i < nfds; i++)
    {
        if (fds[i].fd == -1)
        {
            for (int j = i; j < nfds; j++)
            {
                fds[j].fd = fds[j + 1].fd;
            }
            i--;
            nfds--;
        }
    }
}

UniSocket FDSetWrapper::sockAt(const int &index)
{
    return UniSocket(fds[index].fd);
}

vector<UniSocket> FDSetWrapper::getReadySockets()
{
    vector<UniSocket> readySockets;
    int result = poll(fds, static_cast<nfds_t>(nfds), timeout);
    if(result < 0)
        throw UniSocketException(UniSocketException::POLL);

    if(result == 0)
        throw UniSocketException(UniSocketException::TIMED_OUT);

    current_size = nfds;
    for(int i = 0; i < current_size;i++)
    {
        if(fds[i].revents == 0)
            continue;

        if(fds[i].revents != POLLIN)
            throw UniSocketException(UniSocketException::POLL);

        readySockets.emplace_back(fds[i].fd);
    }
    return readySockets;
}

FDSetWrapper::FDSetWrapper()
{}

vector<UniSocket> FDSetWrapper::getAllFDS()
{
    vector<UniSocket> allSocks;
    current_size = nfds;
    for(int i = 0; i < current_size;i++)
    {
        readySockets.emplace_back(fds[i].fd);
    }
}