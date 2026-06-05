#pragma once

#include <sys/socket.h>
#include <unistd.h>

#include <iostream>
#include <string>

#include "endpoint.hpp"
#include "socket.hpp"

class Connection {
protected:
    Endpoint ENDPOINT;
    Socket SOCKET;

public:
    Connection() = default;
    Connection(const std::string& ip, int port)
        : ENDPOINT(ip, port) {}
    Connection(int socket_fd, const sockaddr_in& addr)
        : ENDPOINT(addr), SOCKET(socket_fd) {}

    ~Connection() = default;

    // Getters
    const Endpoint& getEndpoint() const {
        return ENDPOINT;
    }
    const Socket& getSocket() const {
        return SOCKET;
    }
    Endpoint& getEndpoint() {
        return ENDPOINT;
    }
    Socket& getSocket() {
        return SOCKET;
    }

    // Data transfer
    bool sendData(const std::string& message) {
        if (!isValid()) {
            return false;
        }

        ssize_t sent = send(
            SOCKET.getFD(),
            message.data(),
            message.size(),
            0);

        return sent != -1;
    }
    std::string receiveData() {
        if (!isValid()) {
            return "";
        }

        char buffer[1024];

        ssize_t bytes = recv(
            SOCKET.getFD(),
            buffer,
            sizeof(buffer),
            0);

        if (bytes == 0) {
            return "";
        }

        return std::string(buffer, bytes);
    }

    // Helpers
    bool isValid() const {
        return ENDPOINT.isValidIP() && SOCKET.isValid();
    }
};