#pragma once

#include <sys/socket.h>

#include <iostream>

#include "core/network/connection.hpp"
#include "core/network/endpoint.hpp"
#include "core/network/socket.hpp"

class Server {
private:
    Socket LISTEN_SOCKET;
    Endpoint ENDPOINT;

public:
    Server() {
        std::cout
            << "[Server] Default server created."
            << std::endl;
    }

    Server(const std::string& ip, uint16_t port)
        : ENDPOINT(ip, port) {
        std::cout
            << "[Server] Server created for "
            << ip
            << ":"
            << port
            << std::endl;
    }

    ~Server() = default;

    // Getters
    const Socket& getListeningSocket() const {
        return LISTEN_SOCKET;
    }

    // Server Lifecycle
    bool createListenSocket() {
        std::cout
            << "[Server] Creating listening socket..."
            << std::endl;

        bool result =
            LISTEN_SOCKET.createSocket();

        if (result) {
            std::cout
                << "[Server] Listening socket created. FD: "
                << LISTEN_SOCKET.getFD()
                << std::endl;
        }

        return result;
    }

    bool bindListenSocket() {
        std::cout
            << "[Server] Binding to "
            << ENDPOINT.getIPAddress()
            << ":"
            << ENDPOINT.getPort()
            << std::endl;

        if (!LISTEN_SOCKET.isValid()) {
            std::cerr
                << "[Server] Invalid listening socket."
                << std::endl;
            return false;
        }

        sockaddr_in addr =
            ENDPOINT.getSockAddr();

        bool bound =
            bind(
                LISTEN_SOCKET.getFD(),
                reinterpret_cast<sockaddr*>(&addr),
                sizeof(addr)) != -1;

        if (bound) {
            std::cout
                << "[Server] Bind successful."
                << std::endl;
        } else {
            std::cerr
                << "[Server] Bind failed."
                << std::endl;
        }

        return bound;
    }

    bool startListening(int backlog = 5) {
        std::cout
            << "[Server] Starting listener. Backlog: "
            << backlog
            << std::endl;

        if (!LISTEN_SOCKET.isValid()) {
            std::cerr
                << "[Server] Invalid listening socket."
                << std::endl;
            return false;
        }

        bool listening =
            listen(
                LISTEN_SOCKET.getFD(),
                backlog) != -1;

        if (listening) {
            std::cout
                << "[Server] Listening for incoming connections."
                << std::endl;
        } else {
            std::cerr
                << "[Server] listen() failed."
                << std::endl;
        }

        return listening;
    }

    // Clients Handling
    Connection acceptConnection() {
        std::cout
            << "[Server] Waiting for client..."
            << std::endl;

        sockaddr_in client_addr{};
        socklen_t addr_len = sizeof(client_addr);

        int client_fd = accept(
            LISTEN_SOCKET.getFD(),
            reinterpret_cast<sockaddr*>(&client_addr),
            &addr_len);

        if (client_fd == -1) {
            std::cerr
                << "[Server] accept() failed."
                << std::endl;

            return Connection();
        }

        std::cout
            << "[Server] Client accepted. FD: "
            << client_fd
            << std::endl;

        return Connection(
            client_fd,
            client_addr);
    }

    void handleClient(Connection conn);
    void run();
};