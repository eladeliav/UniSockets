//
// Created by elade on 3/16/2019.
//
#include "UniSocketException.h"
#include <iostream>

std::ostream &operator<<(std::ostream &os, const UniSocketException &uniSockExcept)
{
    os << uniSockExcept.ERROR_MESSAGES.at(uniSockExcept._errorType);
    return os;
}

UniSocketException::UniSocketException(UniSocketException::ErrorType type)
{
    this->_errorType = type;
}

UniSocketException::ErrorType UniSocketException::getError()
{
    return this->_errorType;
}