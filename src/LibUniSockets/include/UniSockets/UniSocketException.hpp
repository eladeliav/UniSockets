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
    const map<ErrorType, std::string> ERROR_MESSAGES = MapCreate::create_map();
private:
    ErrorType _errorType;
public:
    explicit UniSocketException(ErrorType type);

    ErrorType getError();

    const char *what() const noexcept override
    {
        return UniSocketException::ERROR_MESSAGES.at(this->_errorType).c_str();
    }

    friend std::ostream &operator<<(std::ostream &os, const UniSocketException &uniSockExcept);
};

#endif