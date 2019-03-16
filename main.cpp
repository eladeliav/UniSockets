//
// Created by Elad Eliav on 2019-03-15.
//
#include <iostream>
#include <string>
#include "UniSocket.h"
#include "UniSocketSet.h"
#include <vector>

#define DEFAULT_PORT 5400
#define DEFAULT_IP "127.0.0.1"
#define DEFAULT_BUFFER_LEN 1024
#define LOG(x) std::cout << x << std::endl
#define WELCOME_MSG "Welcome to the chat room\r\n"

using std::string;

int main()
{
    UniSocket listenSock(5400, SOMAXCONN);

    UniSocketSet set;
    set.addSock(listenSock);
    bool running = true;
    int result = 0;
    string receivedString;

    while(running)
    {
        int socketCount = set.select();
        for(int i = 0; i < socketCount;i++)
        {
            UniSocket currentSock = UniSocket(set.sockAt(i));
            if(listenSock == currentSock)
            {
                UniSocket newClient = listenSock.accept(result);
                if(result <= -1)
                    continue;
                set.addSock(newClient);
                newClient.send(WELCOME_MSG);
                LOG("Someone Has Joined!");
                set.broadcast("Someone Has Joined!\r\n", newClient);
            } else
            {
                receivedString = currentSock.recv(result);
                if(result <= 0)
                {
                    LOG("Someone has left!");
                    set.removeSock(currentSock);
                    set.broadcast("Someone Has Left!\r\n", currentSock);
                } else
                {
                    LOG("Someone wrote: " + receivedString);
                    string msg = "Someone wrote: " + receivedString;
                    set.broadcast(msg, currentSock);
                }
            }
        }
    }
}