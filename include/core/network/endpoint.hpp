#pragma once

#include <arpa/inet.h>

#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>

class Endpoint {
protected:
    std::string IP_ADDRESS;
    uint16_t PORT;

public:
    Endpoint()
        : IP_ADDRESS("127.0.0.1"),
          PORT(8080) {
        std::cout
            << "[Endpoint] Default endpoint created: "
            << IP_ADDRESS << ":" << PORT
            << std::endl;
    }

    Endpoint(const std::string& IP_ADDRESS,
             const uint16_t PORT)
        : IP_ADDRESS(IP_ADDRESS),
          PORT(PORT) {
        std::cout
            << "[Endpoint] Endpoint created: "
            << this->IP_ADDRESS << ":"
            << this->PORT
            << std::endl;
    }

    Endpoint(const sockaddr_in& addr) {
        char ip[INET_ADDRSTRLEN];

        inet_ntop(
            AF_INET,
            &addr.sin_addr,
            ip,
            sizeof(ip));

        this->IP_ADDRESS = ip;
        this->PORT = ntohs(addr.sin_port);

        std::cout
            << "[Endpoint] Created from sockaddr_in: "
            << IP_ADDRESS << ":"
            << PORT
            << std::endl;
    }

    virtual ~Endpoint() = default;

    // ------------ Getters ---------------
    const std::string& getIPAddress() const {
        return IP_ADDRESS;
    }

    uint16_t getPort() const {
        return PORT;
    }

    // ------------ Helpers ---------------
    sockaddr_in getSockAddr() const {
        sockaddr_in ADDR{};

        ADDR.sin_family = AF_INET;
        ADDR.sin_port = htons(PORT);

        if (!isValidIP()) {
            std::cerr
                << "[Endpoint] Invalid IP Address: "
                << IP_ADDRESS
                << std::endl;

            throw std::runtime_error(
                "Invalid IP Address");
        }

        inet_pton(
            AF_INET,
            IP_ADDRESS.c_str(),
            &ADDR.sin_addr);

        std::cout
            << "[Endpoint] Converted to sockaddr_in: "
            << IP_ADDRESS << ":"
            << PORT
            << std::endl;

        return ADDR;
    }

    bool isValidIP() const {
        sockaddr_in addr;

        return inet_pton(
                   AF_INET,
                   IP_ADDRESS.c_str(),
                   &addr.sin_addr) == 1;
    }
};