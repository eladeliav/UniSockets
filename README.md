<!--
*** Thanks for checking out this README Template. If you have a suggestion that would
*** make this better please fork the repo and create a pull request or simple open
*** an issue with the tag "enhancement".
*** Thanks again! Now go create something AMAZING! :D
-->





<!-- PROJECT SHIELDS -->
[![Build Status][build-shield]]()
[![Contributors][contributors-shield]]()


<!-- PROJECT LOGO -->
<br />
<p align="center">
  <a href="https://github.com/eladeliav/UniSockets">
    <img src="https://cdn2.iconfinder.com/data/icons/seo-internet-marketing-6/256/Communities__Networks-512.png" alt="Logo" width="80" height="80">
  </a>

  <h1 align="center">UniSockets</h1>

  <p align="center">
    A simple cross-platform sockets library for c++.
    <br />
    This project was made as part of my 12th grade software engineering project.
    See my <a href="https://github.com/eladeliav/UniSockets">github profile</a> for the full project.
    <br />
    <!--<a href="https://github.com/othneildrew/Best-README-Template"><strong>Explore the docs »</strong></a>
    <br />
    <br />
    <a href="https://github.com/othneildrew/Best-README-Template">View Demo</a>
    ·
    <a href="https://github.com/othneildrew/Best-README-Template/issues">Report Bug</a>
    ·
    <a href="https://github.com/othneildrew/Best-README-Template/issues">Request Feature</a>
    -->
  </p>
</p>



<!-- TABLE OF CONTENTS -->
## Table of Contents

* [About the Project](#about-the-project)
  * [Built With](#built-with)
* [Getting Started](#getting-started)
* [Usage](#usage)
* [Goals](#goals)
* [License](#license)
* [Contact](#contact)
* [Acknowledgements](#acknowledgements)



<!-- ABOUT THE PROJECT -->
## About The Project
<!--[![Product Name Screen Shot][product-screenshot]](https://example.com))-->

This project is basically a library I wrote in c++ as a wrapper for winsock and BSD sockets. 
My UniSockets library basically works as a cross-platform sockets wrapper which is very easy to use and
even includes a decent amount of exception handling. As stated before, this project was made
as part of my 12th grade software engineering project which includes a lot more than just this library.
Please feel free to check out the other parts of the project on my <a href="https://github.com/eladeliav/">github</a>
profile.
### Built With
This section should list any major frameworks that you built your project using. Leave any add-ons/plugins for the acknowledgements section. Here are a few examples.
* [CLion](https://www.jetbrains.com/clion/)
* [CMake](https://cmake.org/)
* [C++](http://www.cplusplus.com/doc/tutorial/)



<!-- GETTING STARTED -->
## Getting Started

This is an example of how you may give instructions on setting up your project locally.
To get a local copy up and running follow these simple example steps.



### Installation

1. Clone the repo
<br />```git clone https:://github.com/eladeliav/UniSockets.git```
2. Use cmake to generate a build folder with either of the following
    * Unix Make:
        - Navigate to ```UniSockets/build``` and run cmake with your needed settings
            ```bash
            cmake ../
            make
            ```
    * Mingw:
        - Navigate to ```UniSockets/build``` and run cmake with your needed settings
            ```bash
            cmake ../
            mingw32-make
            ```
3. Set UniSockets/build as an environment variable named ```UNISOCKETS_DIR```


<!-- USAGE EXAMPLES -->
## Usage

In order to use the library in a CMake project, you have to set the ```UniSockets_DIR``` variable
to the environment variable you setup during installation and then simply run ```find_package(UniSockets)```
and ```target_link_libraries(TARGET_NAME_HERE UniSockets::UniSockets)``` after running ```add_executable/add_library()```.
<br />
##### Example CMakeLsits.txt file:
```cmake
cmake_minimum_required(VERSION 3.0.0)
project(UniSockServer VERSION 0.1.0)

set(TARGET_NAME ${PROJECT_NAME})
set(${TARGET_NAME}_SRC
        server.cpp
        )

set(UniSockets_DIR $ENV{UNISOCKETS_DIR})
find_package(UniSockets REQUIRED)

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SRC})
target_link_libraries(${PROJECT_NAME} UniSockets::UniSockets)
```

##### Example UniSocket Chat Server: 
```cpp
#include <iostream>
#include <string>
#include <sstream>
#include "UniSockets/UniSocket.hpp"
#include <array>
#include <cstring>
#include <vector>
#include <algorithm>
#include <thread>

#define DEFAULT_PORT 5400
#define DEFAULT_IP "127.0.0.1"
#define DEFAULT_BUFFER_LEN 1024
#define LOG(x) std::cout << x << std::endl
#define WELCOME_MSG "Welcome to the chat room!\n"
#define SEND_FILE_COMMAND "SEND_FILE"
#define SENDING_ALERT "SENDING_FILE"

using std::string;
using std::array;
using std::vector;

template<class T>
void Vec_RemoveAll(vector<T>& vec, T val)
{
    vec.erase(std::remove(vec.begin(), vec.end(), val), vec.end());
}

void handleClient(UniSocket client, vector<UniSocket>& allClients, bool& running)
{
    char buf[DEFAULT_BUFFER_LEN];
    while(running)
    {
        memset(buf, '\0', DEFAULT_BUFFER_LEN);
        try
        {
            client.recv(buf);
        }
        catch (UniSocketException &e)
        {
            if(e.getErrorType() != UniSocketException::TIMED_OUT)
            {
                LOG(e);
                LOG("Someone has left!");
                Vec_RemoveAll(allClients, client);
                try
                {
                    UniSocket::broadcastToSet("Someone Has Left!", allClients, false);
                } catch (UniSocketException &e)
                {
                    LOG(e);
                }
                break;
            }
        }

        std::string msg = buf;
        if(msg.empty())
            continue;
        LOG("Someone wrote: " << msg);
        msg = "Someone wrote: " + msg;
        try
        {
            UniSocket::broadcastToSet(msg, allClients, false, client);
        } catch (UniSocketException &e)
        {
            LOG(e);
            LOG("Someone has left!");
            Vec_RemoveAll(allClients, client);
            try
            {
                UniSocket::broadcastToSet("Someone Has Left!", allClients, false);
            } catch (UniSocketException &e)
            {
                LOG(e);
            }
            continue;
        }
    }
}

int main()
{
    UniSocket listenSock(DEFAULT_PORT, SOMAXCONN);
    vector<UniSocket> allClients;
    bool running = true;

    while (running)
    {
        UniSocket newClient = UniSocket();
        try
        {
            newClient = listenSock.acceptIntervals();
        }
        catch(UniSocketException& e)
        {
            if(e.getErrorType() != UniSocketException::TIMED_OUT)
            {
                LOG(e);
                break;
            }
        }

        if(newClient.valid())
        {
            newClient.send(WELCOME_MSG, sizeof(WELCOME_MSG));
            LOG("Someone Has Joined!");
            UniSocket::broadcastToSet("Someone Has Joined!", allClients, false);
            allClients.push_back(newClient);
            std::thread newClnThread = std::thread(handleClient, newClient, std::ref(allClients), std::ref(running));
            newClnThread.detach();
        }
    }
    return 0;
}
```

##### Example UniSocket Chat Client: 
```cpp
#include <iostream>
#include "UniSockets/UniSocket.hpp"
#include <thread>

#ifndef _WIN32
#include <cstring>
#endif

using std::string;
using std::thread;
#define DEFAULT_BUFFER_LEN 1024

void sendMessages(UniSocket& sock)
{
    static string userInput;
    static bool connected = true;
    do
    {
        std::cout << "> ";
        std::cin >> std::ws;
        std::getline(std::cin, userInput, '\n');
        if (userInput.size() > 0)
        {
            try
            {
                sock.send(userInput.c_str(), userInput.length());
            }catch(UniSocketException& e)
            {
                connected = false;
            }
        }
    } while (connected);
}

int main()
{
    UniSocket client;
    try
    {
        client = UniSocket("127.0.0.1", 5400);
    }catch(UniSocketException& e)
    {
        std::cout << e << std::endl;
        return 1;
    }

    if(!client.valid())
        return 1;

    std::thread sendMessagesThread(sendMessages, std::ref(client));
    sendMessagesThread.detach();
    char buf[DEFAULT_BUFFER_LEN];
    bool connected = true;
    do
    {
        memset(buf, '\0', DEFAULT_BUFFER_LEN);
        try
        {
            client.recv(buf);
        }catch(UniSocketException& e)
        {
            connected = false;
            continue;
        }
        std::cout << buf << std::endl;
        std::cout << ">";
    } while (connected);
    return 0;
}


```

<!-- GOALS -->
## Goals

As stated before, this library is a work-in-progress for my 12th grade project. So I still have some
goals to accomplish.

- [x] Get rid of UniSocketSet and create ```acceptIntervals()``` instead
- [x] Maybe make the sockets work like c++ streams ( << >>)
- [ ] Consider making a separate class for UniServerSocket
- [x] Somehow merge the socket wrapper classes into one class
- [ ] General code clean up
- [ ] Documentation



<!-- LICENSE -->
## License

Distributed under the MIT License. See LICENSE for more information.


<!-- CONTACT -->
## Contact

Elad Eliav - elad02.eliav@gmail.com

Project Link: [https://github.com/eladeliav/UniSockets](https://github.com/eladeliav/UniSockets)



<!-- ACKNOWLEDGEMENTS -->
## Acknowledgements
* [GitHub Emoji Cheat Sheet](https://www.webpagefx.com/tools/emoji-cheat-sheet)
* [Img Shields](https://shields.io)
* [Choose an Open Source License](https://choosealicense.com)





<!-- MARKDOWN LINKS & IMAGES -->
[build-shield]: https://img.shields.io/badge/build-passing-brightgreen.svg?style=flat-square
[contributors-shield]: https://img.shields.io/badge/contributors-1-orange.svg?style=flat-square
[license-url]: https://choosealicense.com/licenses/mit
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=flat-square&logo=linkedin&colorB=555
[linkedin-url]: https://linkedin.com/in/othneildrew
[product-screenshot]: https://raw.githubusercontent.com/othneildrew/Best-README-Template/master/screenshot.png
