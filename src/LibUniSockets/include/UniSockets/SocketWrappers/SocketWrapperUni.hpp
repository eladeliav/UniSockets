//
// Created by elad on 6/27/19.
//

#ifndef UNISOCKETS_SOCKETWRAPPERUNI_HPP
#define UNISOCKETS_SOCKETWRAPPERUNI_HPP

#include "UniSockets/SocketWrappers/ISocketWrapper.hpp"
#include "UniSockets/UniSocketException.hpp"

class SocketWrapper: public ISocketWrapper
{
public:
    SocketWrapper() = default; // empty constructor

    SocketWrapper(std::string ip, int port, int timeout); // client constructor

    int send(const void* data, int bufLen) const override; // send with size header

    int raw_send(const void* data, int bufLen) const override; // send data raw (as is)

    int recv(void* buf) const override; // receive data with size header

    int raw_recv(void* buf, int bufLen) const override; // receive data raw (as is)

    int getSockId() const override; // return fd

    void close() override; // close sock

    void setTimeout(int timeout) override; // set timeout for send/recv

    //server constructors
    SocketWrapper(int port, int maxCon, int timeout); // constructor for a server

    SocketWrapper(sockaddr_in address, int sock); //  init a socket wrapper through address info and fd #

    // server functions
    SocketWrapper accept(); // accept a socket (hogs thread)

    SocketWrapper acceptIntervals(); // accepts at intervals according to this->_timeout

    virtual bool valid() override; // checks if socket is valid
private:
    static int numDigits(int num);
    static std::string extractIp(sockaddr_in &address);
#ifdef _WIN32

    bool initWinsock(WSAData& wsaData);
#endif
};


#endif //UNISOCKETS_SOCKETWRAPPERUNI_HPP
