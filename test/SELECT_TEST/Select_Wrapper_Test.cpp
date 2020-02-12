#include <iostream>
#include <string>
#include "UniSockets/Core.hpp"
#include <cstring>
#include <vector>
#include <thread>

#define MESSAGE "THIS IS THE MESSAGE TO SEND ABCDEFGHIJKLMNOPQRSTUVWXYZ123456789"

void client(bool send, bool& done)
{
    UniSocket client("127.0.0.1", 5400);
    if(send)
    {
        client.send(MESSAGE);
    }
    while(!done)
        continue;
    client.close();
}

int main()
{
    UniServerSocket serverSocket = UniServerSocket(5400, SOMAXCONN, 0);
    bool done = false;
    std::vector<UniSocket> sendSocks;
    std::thread cl1(client, true, std::ref(done));
    cl1.detach();
    std::thread cl2(client, false, std::ref(done));
    cl2.detach();
    std::thread cl3(client, true, std::ref(done));
    cl3.detach();
    std::thread cl4(client, false, std::ref(done));
    cl4.detach();
    std::vector<UniSocket> v;
    UniSocket s1 = serverSocket.accept();
    sendSocks.push_back(s1);
    v.push_back(s1);
    v.push_back(serverSocket.accept());
    UniSocket s2 = serverSocket.accept();
    sendSocks.push_back(s2);
    v.push_back(s2);
    v.push_back(serverSocket.accept());
    std::vector<UniSocket> readable = UniSocket::select(v, 0);
    std::cout << readable.size() << std::endl;
    if(readable.size() != sendSocks.size())
        return 1;
    for(size_t i = 0; i < readable.size(); i++)
    {
        if(sendSocks.at(i) != readable.at(i))
            return 2;
    }
    UniSocket::cleanup();
    return 0;
}