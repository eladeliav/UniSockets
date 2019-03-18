//
// Created by Elad Eliav on 2019-03-15.
//
#include <iostream>
#include <string>
#include "UniSocket.h"
#include "UniSocketSet.h"
#include <array>

#define DEFAULT_PORT 5400
#define DEFAULT_IP "127.0.0.1"
#define DEFAULT_BUFFER_LEN 1024
#define LOG(x) std::cout << x << std::endl
#define WELCOME_MSG "Welcome to the chat room!\n"

using std::string;
using std::array;

int main()
{
    UniSocket listenSock(5400, SOMAXCONN);

    UniSocketSet set(listenSock);
    vector<UniSocket> readySockets;
    bool running = true;

    while(running)
    {
        readySockets = set.getReadySockets();
        for(UniSocket& currentSock : readySockets)
        {
            if(listenSock == currentSock)
            {
                UniSocketStruct newClientStruct = listenSock.accept();
                UniSocket newClient = newClientStruct.data;
                if(newClientStruct.errorCode <= 0)
                    continue;
                set.addSock(newClient);
                newClient.send(WELCOME_MSG);
                LOG("Someone Has Joined!");
                set.broadcast("Someone Has Joined!", array<UniSocket, 2>{newClient, listenSock});
            } else
            {
                UniSocketStruct receiveObj = currentSock.recv();
                if(receiveObj.errorCode <= 0)
                {
                    LOG("Someone has left!");
                    set.removeSock(currentSock);
                    set.broadcast("Someone Has Left!", array<UniSocket, 2>{currentSock, listenSock});
                } else
                {
                    LOG("Someone wrote: " + receiveObj.data);
                    string msg = "Someone wrote: " + receiveObj.data;
                    set.broadcast(msg, array<UniSocket, 2>{currentSock, listenSock});
                }
            }
        }
    }
}