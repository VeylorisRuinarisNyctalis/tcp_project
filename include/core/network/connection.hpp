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
    Connection() {
        std::cout
            << "[Connection] Default connection created."
            << std::endl;
    }

    Connection(const std::string& ip, int port)
        : ENDPOINT(ip, port) {
        std::cout
            << "[Connection] Created for "
            << ENDPOINT.getIPAddress()
            << ":"
            << ENDPOINT.getPort()
            << std::endl;
    }

    Connection(int socket_fd, const sockaddr_in& addr)
        : ENDPOINT(addr),
          SOCKET(socket_fd) {
        std::cout
            << "[Connection] Accepted connection from "
            << ENDPOINT.getIPAddress()
            << ":"
            << ENDPOINT.getPort()
            << " (FD: "
            << SOCKET.getFD()
            << ")"
            << std::endl;
    }

    ~Connection() {
        std::cout
            << "[Connection] Destroying connection for "
            << ENDPOINT.getIPAddress()
            << ":"
            << ENDPOINT.getPort()
            << std::endl;
    }

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
            std::cerr
                << "[Connection] Invalid socket."
                << std::endl;
            return false;
        }

        std::cout
            << "[Connection] Sending "
            << message.size()
            << " bytes to "
            << ENDPOINT.getIPAddress()
            << ":"
            << ENDPOINT.getPort()
            << std::endl;

        size_t total = 0;

        while (total < message.size()) {
            ssize_t sent = send(
                SOCKET.getFD(),
                message.data() + total,
                message.size() - total,
                0);

            if (sent <= 0) {
                std::cerr
                    << "[Connection] Send failed."
                    << std::endl;
                return false;
            }

            total += sent;
        }

        std::cout
            << "[Connection] Sent "
            << total
            << " bytes successfully."
            << std::endl;

        return true;
    }

    std::string receiveData() {
        if (!SOCKET.isValid()) {
            std::cerr
                << "[Connection] Invalid socket."
                << std::endl;
            return "NOT-EXISTS";
        }

        char buffer[1024];

        ssize_t bytes = recv(
            SOCKET.getFD(),
            buffer,
            sizeof(buffer),
            0);

        if (bytes == -1) {
            std::cerr
                << "[Connection] recv() failed."
                << std::endl;

            return "FAILED";
        }

        if (bytes == 0) {
            std::cout
                << "[Connection] Peer disconnected: "
                << ENDPOINT.getIPAddress()
                << ":"
                << ENDPOINT.getPort()
                << std::endl;

            return "DISCONNECTED";
        }

        std::cout
            << "[Connection] Received "
            << bytes
            << " bytes from "
            << ENDPOINT.getIPAddress()
            << ":"
            << ENDPOINT.getPort()
            << std::endl;

        return std::string(buffer, bytes);
    }
};