#pragma once

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>

class Socket {
protected:
    int SOCKET_FD;

public:
    Socket() : SOCKET_FD(-1) {}

    ~Socket() {
        closeSocket();
    }

    // Returns true, if creates Socket, else false
    bool createSocket() {
        if (SOCKET_FD >= 0) {
            return false;
        }

        SOCKET_FD = socket(AF_INET, SOCK_STREAM, 0);

        if (SOCKET_FD < 0) {
            std::cerr << "Socket not created." << std::endl;
            return false;
        }

        return true;
    }
    // Returns true, if closes an existing Socket, else false
    bool closeSocket() {
        if (SOCKET_FD < 0) {
            return false;
        }

        ::close(SOCKET_FD);
        SOCKET_FD = -1;

        return true;
    }

    // Returns the socket value
    int getFD() const {
        return this->SOCKET_FD;
    }
};