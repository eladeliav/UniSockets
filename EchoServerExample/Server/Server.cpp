//
// Created by Elad Eliav on 2019-03-15.
//
#include <iostream>
#include "UniSocket.h"

using std::string;

int main()
{
    try
    {
        UniSocket uniSock(2525, SOMAXCONN); //listen sock
        UniSocket client = uniSock.accept(); // accepted client socket
        string received;
        do
        {
            received = client.recv();
            std::cout << received << std::endl;
            client.send(received);
        } while (received != "EXIT");
    } catch (UniSocketException e)
    {
        std::cout << e << std::endl;
    }
    return 0;
}