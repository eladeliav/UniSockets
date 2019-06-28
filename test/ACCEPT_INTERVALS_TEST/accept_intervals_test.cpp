#include <iostream>
#include <string>
#include "UniSockets/Core.hpp"
#include <cstring>
#include <chrono>

#define MESSAGE "THIS IS THE MESSAGE TO SEND ABCDEFGHIJKLMNOPQRSTUVWXYZ123456789"
#define TIMEOUT 1
using namespace std::chrono;

int main()
{
    UniServerSocket listenSock(5400, SOMAXCONN, TIMEOUT); // init server socket
    auto start = high_resolution_clock::now();
    try
    {
        listenSock.acceptIntervals();
    }catch(UniSocketException& e)
    {
        if(e.getErrorType() == UniSocketException::TIMED_OUT)
        {
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<seconds>(stop - start);
            if(duration.count() != TIMEOUT)
            {
                listenSock.close();
                UniSocket::cleanup();
                exit(1);
            }
        }
    }
    listenSock.close();
    UniSocket::cleanup();
    return 0;
}