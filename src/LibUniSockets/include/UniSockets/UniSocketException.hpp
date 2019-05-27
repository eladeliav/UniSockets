//
// Created by elade on 3/16/2019.
//

#pragma once

#include <string>
#include <map>

using std::map;

class UniSocketException : public std::exception
{
public:
    enum ErrorType
    {
        TIMED_OUT = -15,
        ACCEPT = -5,
        SEND = -1,
        BIND = -7,
        LISTEN = -9,
        WINSOCK = -32,
        SOCKET_INIT = -400,
        CONNECT = -60,
        DISCONNECTED = 0,
        POLL = -123
    };
    const map<ErrorType, std::string> ERROR_MESSAGES = {
            {TIMED_OUT,    "Socket timed out while receiving"},
            {ACCEPT,       "Socket failed to accept"},
            {SEND,         "Socket failed to send message"},
            {BIND,         "Socket failed to bind"},
            {LISTEN,       "Socket failed to listen"},
            {WINSOCK,      "Failed to initialize winsock"},
            {SOCKET_INIT,  "Failed to initialize socket"},
            {CONNECT,      "Socket failed to connect"},
            {DISCONNECTED, "Disconnected"},
            {POLL, "Failed to find readable fds"}
    };
private:
    ErrorType _errorType;
public:

    UniSocketException(ErrorType type);

    ErrorType getError();

    friend std::ostream &operator<<(std::ostream &os, const UniSocketException &uniSockExcept);
};