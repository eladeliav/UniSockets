#include <iostream>
#include <string>
#include "UniSockets/Core.hpp"
#include <cstring>
#include <thread>
#include <chrono>

#define MESSAGE "THIS IS THE MESSAGE TO SEND ABCDEFGHIJKLMNOPQRSTUVWXYZ123456789"
#define TIMEOUT 2

void client()
{
    UniSocket client("127.0.0.1", 5400);
    char buf[DEFAULT_BUFFER_LEN];
    std::clock_t start;
    double duration = 0;
    start = std::clock();
    try
    {
        client.recv(buf);
    }catch(UniSocketException& e)
    {
        if(e.getErrorType() == UniSocketException::TIMED_OUT)
        {
            duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
            int integerVer = duration;
            if(integerVer != TIMEOUT)
            {
                client.close();
                UniSocket::cleanup();
                exit(1);
            }
        }
    }


    client.close();
}

int main()
{
    UniServerSocket listenSock(5400, SOMAXCONN); // init server socket
    std::thread clnThread = std::thread(client);
    clnThread.detach();
    UniSocket newClient = listenSock.accept();
    newClient.setTimeout(TIMEOUT);
    std::this_thread::sleep_for(std::chrono::seconds(TIMEOUT));
    newClient.close();
    listenSock.close();
    UniSocket::cleanup();
    return 0;
}