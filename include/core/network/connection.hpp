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

        ssize_t total = 0;
        while (total < message.size()) {
            ssize_t sent = send(
                SOCKET.getFD(),
                message.data(),
                message.size(),
                0);

            if (sent <= 0) {
                return false;
            }

            total += sent;
        }

        return sent != -1;
    }
    std::string receiveData() {
        if (!isValid()) {
            throw std::runtime_error("invalid socket");
        }

        char buffer[1024];

        ssize_t bytes = recv(
            SOCKET.getFD(),
            buffer,
            sizeof(buffer),
            0);

        if (bytes == -1) {
            throw std::runtime_error("recv failed");
        }
        if (bytes == 0) {
            return "";
        }

        return std::string(buffer, bytes);
    }
};