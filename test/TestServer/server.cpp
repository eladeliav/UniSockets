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


int main()
{
    UniServerSocket listenSock(DEFAULT_PORT, SOMAXCONN); // init server socket
    UniSocket newClient = listenSock.accept();
    newClient.send("HELLO THIS IS A TEST");
    newClient << "HELLO THIS IS A TEST 2 (raw)";
    newClient.close();
    listenSock.close();
    UniSocket::cleanup();
    return 0;
}