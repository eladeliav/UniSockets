//
// Created by Elad Eliav on 2019-03-18.
//

#pragma once
#include "UniSockets/UniSocket.hpp"
#include <vector>
using std::vector;

class FDSetWrapper
{
private:
    fd_set _master;
    fd_set _copy;
    int _masterFd;

    int select();

    int _timeout;
public:

    FDSetWrapper(const UniSocket& masterSock);

    FDSetWrapper();

    void clearSet();

    void addSock(const UniSocket &sock);

    void removeSock(const UniSocket &sock);

    UniSocket sockAt(const int &index);

    vector<UniSocket> getReadySockets();

    vector<UniSocket> getAllFDS();

    friend class UniSocketSet;
};