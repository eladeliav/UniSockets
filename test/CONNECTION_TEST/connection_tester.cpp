#include <iostream>
#include <string>
#include "UniSockets/Core.hpp"
#include <cstring>
#include <thread>

#define MESSAGE "THIS IS THE MESSAGE TO SEND ABCDEFGHIJKLMNOPQRSTUVWXYZ123456789"

void client()
{
    UniSocket client("127.0.0.1", 5400);
    client.close();
}

int main()
{
    UniServerSocket listenSock(5400, SOMAXCONN); // init server socket
    std::thread clnThread = std::thread(client);
    clnThread.detach();
    UniSocket newClient = listenSock.accept();
    newClient.close();
    listenSock.close();
    UniSocket::cleanup();
    return 0;
}