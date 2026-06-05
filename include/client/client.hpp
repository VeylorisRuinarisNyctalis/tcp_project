#pragma once

#include <sys/socket.h>

#include <iostream>
#include <string>

#include "connection.hpp"

class Client {
private:
    Connection CONNECTION;

public:
    Client() = default;
    Client(const std::string& ip, uint16_t port) : CONNECTION(ip, port) {}

    ~Client() = default;

    // Lifecycle
    bool createClientSocket() {
        return CONNECTION.getSocket().createSocket();
    }

    bool connectToServer() {
        if (!CONNECTION.isValid()) {
            return false;
        }

        sockaddr_in client_addr = CONNECTION.getEndpoint().getSockAddr();

        return connect(
                   CONNECTION.getSocket().getFD(),
                   reinterpret_cast<sockaddr*>(&client_addr),
                   sizeof(client_addr)) != -1;
    }
};