//
// Created by elade on 3/16/2019.
//

#pragma once
#ifndef UNISOCKETEXCEPTION_H
#define UNISOCKETEXCEPTION_H
#include <string>
#include <map>
#include <functional>

using std::map;

class UniSocketException
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

    struct MapCreate
    {
        static map<ErrorType, std::string> create_map()
        {
            map<UniSocketException::ErrorType, std::string> m;
            m[TIMED_OUT] = "Socket timed out while receiving";
            m[ACCEPT] = "Socket failed to accept";
            m[SEND] = "Socket failed to send message";
            m[BIND] = "Socket failed to bind";
            m[LISTEN] = "Socket failed to listen";
            m[WINSOCK] = "Failed to initialize winsock";
            m[SOCKET_INIT] = "Failed to initialize socket";
            m[CONNECT] = "Socket failed to connect";
            m[DISCONNECTED] = "Disconnected";
            m[POLL] = "Failed to find readable fds";
            return m;
        }
    };

//    const map<ErrorType, std::string> ERROR_MESSAGES = {
//            {TIMED_OUT,    "Socket timed out while receiving"},
//            {ACCEPT,       "Socket failed to accept"},
//            {SEND,         "Socket failed to send message"},
//            {BIND,         "Socket failed to bind"},
//            {LISTEN,       "Socket failed to listen"},
//            {WINSOCK,      "Failed to initialize winsock"},
//            {SOCKET_INIT,  "Failed to initialize socket"},
//            {CONNECT,      "Socket failed to connect"},
//            {DISCONNECTED, "Disconnected"},
//            {POLL,         "Failed to find readable fds"}
//    };
    const map<ErrorType, std::string> ERROR_MESSAGES = MapCreate::create_map();
private:
    ErrorType _errorType;
public:

    UniSocketException(ErrorType type);

    ErrorType getError();

    friend std::ostream &operator<<(std::ostream &os, const UniSocketException &uniSockExcept);
};
#endif