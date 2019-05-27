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
    struct pollfd fds[200];
    int timeout = 3;
    int nfds = 1;
    int current_size = 0;
    vector<UniSocket> getAllFDS();
public:
    FDSetWrapper(const UniSocket& masterSock);
    FDSetWrapper(const UniSocket &masterSock, const int& timeout);

    FDSetWrapper();

    void clearSet();

    void addSock(const UniSocket &sock);

    void removeSock(const UniSocket &sock);

    UniSocket sockAt(const int &index);

    vector<UniSocket> getReadySockets();

    friend class UniSocketSet;
};

