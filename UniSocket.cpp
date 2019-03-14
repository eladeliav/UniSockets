//
// Created by elade on 3/14/2019.
//

#include "UniSocket.h"

UniSocketException::UniSocketException(std::string errorMsg)
{
    this->_errorMsg = errorMsg;
}

UniSocket::UniSocket()
{
    
}