#pragma once

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>

class Socket {
protected:
    int SOCKET_FD;

public:
    Socket() : SOCKET_FD(-1) {
        std::cout << "[Socket] Default constructor called." << std::endl;
    }

    Socket(int socket_fd) : SOCKET_FD(socket_fd) {
        std::cout << "[Socket] Constructed with FD: "
                  << SOCKET_FD << std::endl;
    }

    ~Socket() {
        std::cout << "[Socket] Destructor called for FD: "
                  << SOCKET_FD << std::endl;

        closeSocket();
    }

    // Returns true, if creates Socket, else false
    bool createSocket() {
        std::cout << "[Socket] Creating socket..." << std::endl;

        if (isValid()) {
            std::cerr << "[Socket] Socket already created. FD: "
                      << SOCKET_FD << std::endl;
            return false;
        }

        SOCKET_FD = socket(AF_INET, SOCK_STREAM, 0);

        if (!isValid()) {
            std::cerr << "[Socket] Failed to create socket."
                      << std::endl;
            return false;
        }

        std::cout << "[Socket] Socket created successfully. FD: "
                  << SOCKET_FD << std::endl;

        return true;
    }

    // Returns true, if closes an existing Socket, else false
    bool closeSocket() {
        std::cout << "[Socket] Attempting to close socket. FD: "
                  << SOCKET_FD << std::endl;

        if (!isValid()) {
            std::cerr << "[Socket] Socket not created." << std::endl;
            return false;
        }

        std::cout << "[Socket] Closing socket. FD: "
                  << SOCKET_FD << std::endl;

        ::close(SOCKET_FD);

        SOCKET_FD = -1;

        std::cout << "[Socket] Socket closed." << std::endl;

        return true;
    }

    // Returns the socket value
    int getFD() const {
        return SOCKET_FD;
    }

    void setFD(int socket_fd) {
        std::cout << "[Socket] FD changed from "
                  << SOCKET_FD
                  << " to "
                  << socket_fd
                  << std::endl;

        SOCKET_FD = socket_fd;
    }

    // Helpers
    bool isValid() const {
        return SOCKET_FD != -1;
    }
};