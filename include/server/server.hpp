#pragma once

#include <sys/socket.h>

#include "core/network/connection.hpp"
#include "core/network/endpoint.hpp"
#include "core/network/socket.hpp"

class Server {
private:
    Socket LISTEN_SOCKET;
    Endpoint ENDPOINT;

public:
    Server() = default;
    Server(const std::string& ip, uint16_t port) : ENDPOINT(ip, port) {}

    ~Server() = default;

    // Getters
    const Socket& getListeningSocket() const {
        return LISTEN_SOCKET;
    }

    // Server Lifecycle
    bool createListenSocket() {
        return LISTEN_SOCKET.createSocket();
    }
    bool bindListenSocket() {
        if (!LISTEN_SOCKET.isValid()) {
            return false;
        }

        sockaddr_in addr = ENDPOINT.getSockAddr();

        return bind(
                   LISTEN_SOCKET.getFD(),
                   reinterpret_cast<sockaddr*>(&addr),
                   sizeof(addr)) != -1;
    }
    bool startListening(int backlog = 5) {
        if (!LISTEN_SOCKET.isValid()) {
            return false;
        }

        return listen(
                   LISTEN_SOCKET.getFD(),
                   backlog) != -1;
    }

    // Clients Handling
    Connection acceptConnection() {
        sockaddr_in client_addr{};
        socklen_t addr_len = sizeof(client_addr);

        int client_fd = accept(
            LISTEN_SOCKET.getFD(),
            reinterpret_cast<sockaddr*> (&client_addr),
            &addr_len);

        if (client_fd == -1) {
            return Connection();
        }

        return Connection(client_fd, client_addr);
    }

    void handleClient(Connection conn);
    void run();
};