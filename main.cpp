#include <iostream>
#include "UniSocket.h"

using std::string;

int main()
{
    try
    {
        UniSocket client("127.0.0.1", 2600);
        string sendString;

        do
        {
            std::cout << ">";
            std::cin >> sendString;
            client.send(sendString);
            std::cout << "The Server sent back: " << client.recv() << std::endl;
        }while(sendString != "EXIT");
        client.close();
    }catch(UniSocketException e)
    {
        std::cout << e << std::endl;
    }


    return 0;
}