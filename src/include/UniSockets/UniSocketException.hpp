//
// Created by elade on 3/16/2019.
//

#pragma once
#ifndef UNISOCKETEXCEPTION_H
#define UNISOCKETEXCEPTION_H

#include <string>
#include <map>
#include <functional>

class UniSocketException : public std::exception
{
public:
    enum ErrorType
    {
        TIMED_OUT = 100,
        ACCEPT = 101,
        SEND = 102,
        BIND = 103,
        LISTEN = 104,
        WINSOCK = 105,
        SOCKET_INIT = 106,
        CONNECT = 107,
        DISCONNECTED = 108,
        POLL = 109,
        SOCKET_CLOSE = 110,
        BAD_SIZE_HEADER = 111,
    };

    struct MapCreate
    {
        static std::map<ErrorType, std::string> create_map()
        {
            std::map<UniSocketException::ErrorType, std::string> m;
            m[TIMED_OUT] = "Socket timed out";
            m[ACCEPT] = "Socket failed to accept";
            m[SEND] = "Socket failed to send message";
            m[BIND] = "Socket failed to bind";
            m[LISTEN] = "Socket failed to listen";
            m[WINSOCK] = "Failed to initialize winsock";
            m[SOCKET_INIT] = "Failed to initialize socket";
            m[CONNECT] = "Socket failed to connect";
            m[DISCONNECTED] = "Disconnected";
            m[POLL] = "Failed to find readable fds";
            m[SOCKET_CLOSE] = "Socket failed to close";
            m[BAD_SIZE_HEADER] = "Bad size header detected while receiving";
            return m;
        }
    };
    const std::map<ErrorType, std::string> ERROR_MESSAGES = MapCreate::create_map();
private:
    ErrorType _errorType;
public:
    explicit UniSocketException(ErrorType type);

    std::string getError();

    ErrorType getErrorType();

    const char *what() const noexcept override
    {
        return UniSocketException::ERROR_MESSAGES.at(this->_errorType).c_str();
    }

    friend std::ostream &operator<<(std::ostream &os, const UniSocketException &uniSockExcept);
};

#endif