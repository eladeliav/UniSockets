#include <iostream>
#include <string>
#include <sstream>
#include "UniSockets/Core.hpp"
#include <array>
#include <cstring>
#include <vector>
#include <algorithm>
#include <thread>

#define DEFAULT_PORT 5400
#define LOG(x) std::cout << x << std::endl
#define WELCOME_MSG "Welcome to the chat room!\n"

using std::string;
using std::array;
using std::vector;

template<class T>
void Vec_RemoveAll(vector<T>& vec, T val)
{
    vec.erase(std::remove(vec.begin(), vec.end(), val), vec.end());
}

void closeClients(vector<UniSocket>& clients)
{
    for(auto& cl : clients)
        cl.close();
}

void handleClient(UniSocket client, vector<UniSocket>& allClients, bool& running)
{
    char buf[DEFAULT_BUFFER_LEN];
    while(running)
    {
        memset(buf, '\0', DEFAULT_BUFFER_LEN);
        try
        {
            client >> buf; // receive a message
        }
        catch (UniSocketException &e)
        {
            if(e.getErrorType() != UniSocketException::TIMED_OUT) // ignore time outs
            {
                // if it isn't a time out, then the client disconnected
                LOG(e);
                LOG("Someone has left!");
                Vec_RemoveAll(allClients, client); // remove from clients vector
                UniSocket::broadcastToSet("Someone Has Left!", allClients, true); // tell the other clients
                client.close();
                break;
            }
        }

        std::string msg = buf;
        LOG("Someone wrote: " << msg);
        msg = "Someone wrote: " + msg; // creating msg for other clients to see
        UniSocket::broadcastToSet(msg, allClients, true, client); // broadcasting message to all other clients
    }
}

int main()
{
    UniServerSocket listenSock(DEFAULT_PORT, SOMAXCONN); // init server socket
    vector<UniSocket> allClients; // vector to keep track of clients
    bool running = true;

    while (running)
    {
        UniSocket newClient = UniSocket();
        try
        {
            newClient = listenSock.acceptIntervals(); // checking every TIMEOUT seconds for a new client
        }
        catch(UniSocketException& e)
        {
            if(e.getErrorType() != UniSocketException::TIMED_OUT) // ignore time outs
            {
                LOG(e);
                break;
            }
        }

        if(newClient.valid()) // found valid client
        {
            newClient << WELCOME_MSG; // send him a welcome msg
            LOG("Someone Has Joined!");
            UniSocket::broadcastToSet("Someone Has Joined!", allClients, true); // tell the other clients
            allClients.push_back(newClient); // add client to vector
            std::thread newClnThread = std::thread(handleClient, newClient, std::ref(allClients), std::ref(running)); // start thread for that client
            newClnThread.detach();
        }
    }
    // close all socks
    listenSock.close();
    closeClients(allClients);
    return 0;
}