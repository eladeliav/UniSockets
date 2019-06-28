#include <iostream>
#include <string>
#include "UniSockets/Core.hpp"
#include <cstring>
#include <ctime>

#define MESSAGE "THIS IS THE MESSAGE TO SEND ABCDEFGHIJKLMNOPQRSTUVWXYZ123456789"

#define TIMEOUT 1

int main()
{
    UniServerSocket listenSock(5400, SOMAXCONN, TIMEOUT); // init server socket
    std::clock_t start;
    double duration = 0;
    start = std::clock();
    try
    {
        listenSock.acceptIntervals();
    }catch(UniSocketException& e)
    {
        if(e.getErrorType() == UniSocketException::TIMED_OUT)
        {
            duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
            int integerVer = duration;
            if(integerVer != TIMEOUT)
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