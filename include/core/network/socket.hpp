#pragma once

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>

class Socket {
protected:
    int SOCKET_FD;

public:
    Socket()
        : SOCKET_FD(-1) {
        std::cout
            << "[Socket] Default constructor called."
            << std::endl;
    }

    explicit Socket(int socket_fd)
        : SOCKET_FD(socket_fd) {
        std::cout
            << "[Socket] Constructed with FD: "
            << SOCKET_FD
            << std::endl;
    }

    // =========================
    // Copy Disabled
    // =========================

    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;

    // =========================
    // Move Enabled
    // =========================

    Socket(Socket&& other) noexcept
        : SOCKET_FD(other.SOCKET_FD) {
        other.SOCKET_FD = -1;
    }

    Socket& operator=(Socket&& other) noexcept {
        if (this != &other) {
            closeSocket();

            SOCKET_FD = other.SOCKET_FD;
            other.SOCKET_FD = -1;
        }

        return *this;
    }

    ~Socket() {
        closeSocket();
    }

    // =========================
    // Lifecycle
    // =========================

    bool createSocket() {
        std::cout
            << "[Socket] Creating socket..."
            << std::endl;

        if (isValid()) {
            std::cerr
                << "[Socket] Socket already exists. FD: "
                << SOCKET_FD
                << std::endl;

            return false;
        }

        SOCKET_FD =
            socket(AF_INET, SOCK_STREAM, 0);

        if (!isValid()) {
            std::cerr
                << "[Socket] Failed to create socket."
                << std::endl;

            return false;
        }

        std::cout
            << "[Socket] Socket created. FD: "
            << SOCKET_FD
            << std::endl;

        return true;
    }

    bool closeSocket() {
        if (!isValid()) {
            return true;
        }

        std::cout
            << "[Socket] Closing FD: "
            << SOCKET_FD
            << std::endl;

        ::close(SOCKET_FD);

        SOCKET_FD = -1;

        return true;
    }

    // =========================
    // Accessors
    // =========================

    int getFD() const {
        return SOCKET_FD;
    }

    void setFD(int socket_fd) {
        if (SOCKET_FD == socket_fd) {
            return;
        }

        closeSocket();

        SOCKET_FD = socket_fd;

        std::cout
            << "[Socket] FD set to "
            << SOCKET_FD
            << std::endl;
    }

    // =========================
    // Helpers
    // =========================

    bool isValid() const {
        return SOCKET_FD != -1;
    }
};