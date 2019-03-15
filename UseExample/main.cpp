#include <iostream>
#include "UniSocket.h"

int main()
{
    std::cout << "Hello, World!" << std::endl;
    try
    {
        UniSocket uniSock(5400, SOMAXCONN);
    } catch (UniSocketException &e)
    {
        std::cout << e._errorMsg << std::endl;
    }


    return 0;
}