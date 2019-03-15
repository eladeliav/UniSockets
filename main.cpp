#include <iostream>
#include "UniSocket.h"

int main()
{
    std::cout << "Hello, World!" << std::endl;
    try
    {
        UniSocket uniSock(5400, SOMAXCONN);
    }catch(UniSocketException& e)
    {
        std::cout << e << std::endl;
    }


    return 0;
}