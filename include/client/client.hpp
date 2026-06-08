#pragma once

#include <sys/socket.h>

#include <iostream>
#include <string>

#include "core/network/connection.hpp"

class Client {
private:
    Connection CONNECTION;

public:
    Client() {
        std::cout
            << "[Client] Default client created."
            << std::endl;
    }

    Client(const std::string& ip, uint16_t port)
        : CONNECTION(ip, port) {
        std::cout
            << "[Client] Client created for "
            << ip
            << ":"
            << port
            << std::endl;
    }

    ~Client() = default;

    // Getters
    const Connection& getConnection() const {
        return CONNECTION;
    }

    Connection& getConnection() {
        return CONNECTION;
    }

    // Lifecycle
    bool createClientSocket() {
        std::cout
            << "[Client] Creating client socket..."
            << std::endl;

        bool result =
            CONNECTION.getSocket().createSocket();

        if (result) {
            std::cout
                << "[Client] Client socket created. FD: "
                << CONNECTION.getSocket().getFD()
                << std::endl;
        }

        return result;
    }

    bool connectToServer() {
        std::cout
            << "[Client] Connecting to "
            << CONNECTION.getEndpoint().getIPAddress()
            << ":"
            << CONNECTION.getEndpoint().getPort()
            << std::endl;

        if (!CONNECTION.getEndpoint().isValidIP()) {
            std::cerr
                << "[Client] Invalid IP address."
                << std::endl;
            return false;
        }

        if (!CONNECTION.getSocket().isValid()) {
            std::cerr
                << "[Client] Invalid socket."
                << std::endl;
            return false;
        }

        sockaddr_in client_addr =
            CONNECTION.getEndpoint().getSockAddr();

        bool connected =
            connect(
                CONNECTION.getSocket().getFD(),
                reinterpret_cast<sockaddr*>(&client_addr),
                sizeof(client_addr)) != -1;

        if (connected) {
            std::cout
                << "[Client] Connected successfully."
                << std::endl;
        } else {
            std::cerr
                << "[Client] Connection failed."
                << std::endl;
        }

        return connected;
    }

    void run();
};