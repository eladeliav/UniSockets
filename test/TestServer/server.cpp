//
// Created by Elad Eliav on 2019-03-15.
//
#include <iostream>
#include <string>
#include <sstream>
#include "LibUniSockets/UniSocket.h"
#include "LibUniSockets/UniSocketSet.h"
#include <array>

#define DEFAULT_PORT 5400
#define DEFAULT_IP "127.0.0.1"
#define DEFAULT_BUFFER_LEN 1024
#define LOG(x) std::cout << x << std::endl
#define WELCOME_MSG "Welcome to the chat room!\n"
#define SEND_FILE_COMMAND "SEND_FILE"
#define SENDING_ALERT "SENDING_FILE"

using std::string;
using std::array;
void splitRequestAndParams(string commandAndParams, string &command, vector<string> &paramsVector)
{
    command = commandAndParams;
    if (commandAndParams.find(' ') != string::npos)
    {
        command = command.erase(commandAndParams.find(' '));

        string params = commandAndParams.substr(commandAndParams.find(' ') + 1);

        std::stringstream test(params);
        std::string segment;
        while (std::getline(test, segment, ' '))
        {
            paramsVector.push_back(segment);
        }
    }
}

int main()
{
    UniSocket listenSock(DEFAULT_PORT, SOMAXCONN);
    UniSocketSet set(listenSock);
    bool running = true;
    char buf[DEFAULT_BUFFER_LEN];

    while (running)
    {
        for (UniSocket &currentSock : set.getReadySockets())
        {
            memset(buf, '\0', DEFAULT_BUFFER_LEN);
            if (listenSock == currentSock)
            {
                UniSocket newClient;
                try
                {
                    newClient = listenSock.accept();
                } catch (UniSocketException &e)
                {
                    LOG(e);
                    continue;
                }
                set.addSock(newClient);
                newClient.send(WELCOME_MSG, sizeof(WELCOME_MSG));
                LOG("Someone Has Joined!");
                set.broadcast("Someone Has Joined!", array<UniSocket, 2>{newClient, listenSock});
            } else
            {
                try
                {
                    currentSock.recv(buf);
                } catch (UniSocketException &e)
                {
                    LOG(e);
                    LOG("Someone has left!");
                    set.removeSock(currentSock);
                    try
                    {
                        set.broadcast("Someone Has Left!", array<UniSocket, 2>{currentSock, listenSock});
                    } catch (UniSocketException &e)
                    {
                        LOG(e);
                    }
                    continue;
                }

                LOG("Someone wrote: " << buf);
                if(string(buf).find(SEND_FILE_COMMAND) != string::npos)
                {
                    //sendFileCommand(string(buf), currentSock);
                }
                string msg = "Someone wrote: " + string(buf);
                try
                {
                    set.broadcast(msg, array<UniSocket, 2>{currentSock, listenSock});
                } catch (UniSocketException &e)
                {
                    LOG(e);
                }
            }
        }
    }
    return 0;
}