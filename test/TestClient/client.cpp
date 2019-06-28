#include <iostream>
#include "UniSockets/UniSocket.hpp"

#ifndef _WIN32
#include <cstring>
#endif

int main()
{
    UniSocket client("127.0.0.1", 5400);
    char buf[DEFAULT_BUFFER_LEN];
    client.recv(buf);
    std::cout << buf << std::endl;
    client >> buf;
    std::cout << buf << std::endl;
    client.close();
    return 0;
}