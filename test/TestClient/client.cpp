#include <iostream>
#include "UniSockets/UniSocket.hpp"
#include <thread>

#ifndef _WIN32
#include <cstring>
#endif

using std::string;
using std::thread;

void sendMessages(UniSocket& sock)
{
    static string userInput;
    static bool connected = true;
    // while connect print prompt, receive input, send input
    do
    {
        std::cout << "> ";
        std::cin >> std::ws;
        std::getline(std::cin, userInput, '\n');
        // if wrote something
        if (userInput.size() > 0)
        {
            try
            {
                sock << userInput; // send it
            }catch(UniSocketException& e)
            {
                connected = false; // error while sending means disconnect so exit
            }
        }
    } while (connected);
}

int main()
{
    UniSocket client;
    try
    {
        client = UniSocket("127.0.0.1", 5400); // connect to server
    }catch(UniSocketException& e)
    {
        std::cout << e << std::endl;
        return 1;
    }

    if(!client.valid()) // failed
        return 1;

    std::thread sendMessagesThread(sendMessages, std::ref(client)); // start send thread
    sendMessagesThread.detach();
    char buf[DEFAULT_BUFFER_LEN]; // define buffer
    bool connected = true;
    // start printing what we receive
    do
    {
        memset(buf, '\0', DEFAULT_BUFFER_LEN);
        try
        {
            client >> buf; // receive to buffer
        }catch(UniSocketException& e)
        {
            connected = false; // had error so disconnect
            continue;
        }
        std::cout << buf << std::endl; // brint buffer
        std::cout << "> ";
    } while (connected);
    client.close();
    return 0;
}