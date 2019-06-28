#include <iostream>
#include <string>
#include "UniSockets/Core.hpp"
#include <cstring>
#include <thread>
#include <chrono>

#define MESSAGE "THIS IS THE MESSAGE TO SEND ABCDEFGHIJKLMNOPQRSTUVWXYZ123456789"
#define TIMEOUT 1

void server(bool& done)
{
    UniServerSocket listenSock(5400, SOMAXCONN); // init server socket
    UniSocket newClient = listenSock.accept();
    while(!done)
    {

    }
}

int main()
{
    bool gotHere = false;
    std::thread clnThread = std::thread(server, std::ref(gotHere));
    clnThread.detach();
    std::this_thread::sleep_for(std::chrono::microseconds(1));
    UniSocket client("127.0.0.1", 5400, TIMEOUT);
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
            gotHere = true;
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
    UniSocket::cleanup();
    if(!gotHere)
        return 1;
    return 0;
}