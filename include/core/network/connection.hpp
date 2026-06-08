#pragma once

#include <cstdint>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <utility>

#include "core/network/endpoint.hpp"
#include "core/network/socket.hpp"
#include "core/protocol/receive_result.hpp"

class Connection {
protected:
    uint64_t ID;
    Endpoint ENDPOINT;
    Socket SOCKET;

public:
    Connection()
        : ID(0) {
        std::cout
            << "[Connection] Default connection created."
            << std::endl;
    }

    Connection(const std::string& ip, int port)
        : ID(0),
          ENDPOINT(ip, port) {
        std::cout
            << "[Connection] Created for "
            << ENDPOINT.getIPAddress()
            << ":"
            << ENDPOINT.getPort()
            << std::endl;
    }

    Connection(uint64_t id,
               const std::string& ip,
               int port)
        : ID(id),
          ENDPOINT(ip, port) {
        std::cout
            << "[Connection] Created for "
            << ENDPOINT.getIPAddress()
            << ":"
            << ENDPOINT.getPort()
            << std::endl;
    }

    Connection(int socket_fd,
               const sockaddr_in& addr)
        : ID(0),
          ENDPOINT(addr),
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

    Connection(uint64_t id,
               int socket_fd,
               const sockaddr_in& addr)
        : ID(id),
          ENDPOINT(addr),
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

    // =========================
    // Copy Disabled
    // =========================

    Connection(const Connection&) = delete;
    Connection& operator=(const Connection&) = delete;

    // =========================
    // Move Enabled
    // =========================

    Connection(Connection&& other) noexcept
        : ID(other.ID),
          ENDPOINT(std::move(other.ENDPOINT)),
          SOCKET(std::move(other.SOCKET)) {
        other.ID = 0;
    }

    Connection& operator=(Connection&& other) noexcept {
        if (this != &other) {
            ID = other.ID;
            ENDPOINT = std::move(other.ENDPOINT);
            SOCKET = std::move(other.SOCKET);

            other.ID = 0;
        }

        return *this;
    }

    ~Connection() {
        std::cout
            << "[Connection "
            << ID
            << "] Destroyed."
            << std::endl;
    }

    // =========================
    // Getters (const)
    // =========================

    const uint64_t& getID() const {
        return ID;
    }

    const Endpoint& getEndpoint() const {
        return ENDPOINT;
    }

    const Socket& getSocket() const {
        return SOCKET;
    }

    // =========================
    // Getters (non-const)
    // =========================

    uint64_t& getID() {
        return ID;
    }

    Endpoint& getEndpoint() {
        return ENDPOINT;
    }

    Socket& getSocket() {
        return SOCKET;
    }

    // =========================
    // Setters
    // =========================

    void setID(uint64_t id) {
        ID = id;
    }

    // =========================
    // Data Transfer
    // =========================

    bool sendData(const std::string& message) {
        if (!SOCKET.isValid()) {
            std::cerr
                << "[Connection] Invalid socket."
                << std::endl;

            return false;
        }

        std::cout
            << "[Connection "
            << ID
            << "] Sending "
            << message.size()
            << " bytes to "
            << ENDPOINT.getIPAddress()
            << ":"
            << ENDPOINT.getPort()
            << std::endl;

        size_t total = 0;

        while (total < message.size()) {
            ssize_t sent =
                send(
                    SOCKET.getFD(),
                    message.data() + total,
                    message.size() - total,
                    0);

            if (sent <= 0) {
                std::cerr
                    << "[Connection "
                    << ID
                    << "] Send failed."
                    << std::endl;

                return false;
            }

            total += sent;
        }

        std::cout
            << "[Connection "
            << ID
            << "] Sent "
            << total
            << " bytes successfully."
            << std::endl;

        return true;
    }

    ReceiveResult receiveData() {
        if (!SOCKET.isValid()) {
            std::cerr
                << "[Connection] Invalid socket."
                << std::endl;

            return {
                ReceiveStatus::InvalidSocket,
                ""
            };
        }

        char buffer[1024];

        ssize_t bytes =
            recv(
                SOCKET.getFD(),
                buffer,
                sizeof(buffer),
                0);

        if (bytes < 0) {
            std::cerr
                << "[Connection "
                << ID
                << "] recv() failed."
                << std::endl;

            return {
                ReceiveStatus::Error,
                ""
            };
        }

        if (bytes == 0) {
            std::cout
                << "[Connection "
                << ID
                << "] Peer disconnected: "
                << ENDPOINT.getIPAddress()
                << ":"
                << ENDPOINT.getPort()
                << std::endl;

            return {
                ReceiveStatus::Disconnected,
                ""
            };
        }

        std::cout
            << "[Connection "
            << ID
            << "] Received "
            << bytes
            << " bytes from "
            << ENDPOINT.getIPAddress()
            << ":"
            << ENDPOINT.getPort()
            << std::endl;

        return {
            ReceiveStatus::Success,
            std::string(buffer, bytes)
        };
    }
};