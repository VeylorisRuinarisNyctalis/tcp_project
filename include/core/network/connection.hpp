#pragma once

#include <sys/socket.h>
#include <unistd.h>

#include <iostream>
#include <string>

#include "core/network/endpoint.hpp"
#include "core/network/socket.hpp"

class Connection {
protected:
    Endpoint ENDPOINT;
    Socket SOCKET;

public:
    Connection() = default;
    Connection(const std::string& ip, int port) : ENDPOINT(ip, port) {}
    Connection(int socket_fd, const sockaddr_in& addr) : ENDPOINT(addr), SOCKET(socket_fd) {}

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
        if (!SOCKET.isValid()) {
            std::cerr << "Socket not created." << std::endl;
            return false;
        }

        size_t total = 0;
        while (total < message.size()) {
            ssize_t sent = send(
                SOCKET.getFD(),
                message.data() + total,
                message.size() - total,
                0);

            if (sent <= 0) {
                std::cerr << "Message not sent." << std::endl;
                return false;
            }

            total += sent;
        }

        return true;
    }
    std::string receiveData() {
        if (!SOCKET.isValid()) {
            std::cerr << "Socket not created." << std::endl;
            return "NOT-EXISTS";
        }

        char buffer[1024];

        ssize_t bytes = recv(
            SOCKET.getFD(),
            buffer,
            sizeof(buffer),
            0);

        if (bytes == -1) {
            std::cerr << "recv failed." << std::endl;
            return "FAILED";
        }
        if (bytes == 0) {
            return "DISCONNECTED";
        }

        return std::string(buffer, bytes);
    }
};