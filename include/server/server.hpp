#pragma once

#include <cstdint>
#include <sys/socket.h>

#include <iostream>

#include "core/network/connection.hpp"
#include "core/network/endpoint.hpp"
#include "core/network/socket.hpp"

class Server {
private:
    uint64_t nextID;

    Socket LISTEN_SOCKET;
    Endpoint ENDPOINT;

public:
    Server()
        : nextID(1) {
        std::cout
            << "[Server] Default server created."
            << std::endl;
    }

    Server(const std::string& ip, uint16_t port)
        : nextID(1),
          ENDPOINT(ip, port) {
        std::cout
            << "[Server] Server created for "
            << ip
            << ":"
            << port
            << std::endl;
    }

    // Disable copy
    Server(const Server&) = delete;
    Server& operator=(const Server&) = delete;

    // Allow move
    Server(Server&&) noexcept = default;
    Server& operator=(Server&&) noexcept = default;

    ~Server() = default;

    // =========================
    // Getters
    // =========================

    const Socket& getListeningSocket() const {
        return LISTEN_SOCKET;
    }

    // =========================
    // Server Lifecycle
    // =========================

    bool createListenSocket() {
        std::cout
            << "[Server] Creating listening socket..."
            << std::endl;

        bool result =
            LISTEN_SOCKET.createSocket();

        if (result) {
            int reuseAddress = 1;

            if (setsockopt(
                    LISTEN_SOCKET.getFD(),
                    SOL_SOCKET,
                    SO_REUSEADDR,
                    &reuseAddress,
                    sizeof(reuseAddress)) == -1) {
                std::cerr
                    << "[Server] Failed to enable address reuse."
                    << std::endl;

                LISTEN_SOCKET.closeSocket();

                return false;
            }

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

    // =========================
    // Client Handling
    // =========================

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

        uint64_t clientID = nextID++;

        std::cout
            << "[Server] Client "
            << clientID
            << " accepted. FD: "
            << client_fd
            << std::endl;

        return Connection(
            clientID,
            client_fd,
            client_addr);
    }

    void handleClient(Connection conn);

    void run();
};
