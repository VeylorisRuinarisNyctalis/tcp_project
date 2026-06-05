#pragma once

#include <arpa/inet.h>

#include <cstdint>
#include <iostream>
#include <string>

class Endpoint {
protected:
    std::string IP_ADDRESS;
    uint16_t PORT;

public:
    Endpoint() : IP_ADDRESS("127.0.0.1"), PORT(8080) {}
    Endpoint(const std::string& IP_ADDRESS, const uint16_t& PORT) : IP_ADDRESS(IP_ADDRESS), PORT(PORT) {}

    virtual ~Endpoint() = default;

    // ------------ Getters ---------------
    const std::string& getIPAddress() const {
        return this->IP_ADDRESS;
    }
    uint16_t getPort() const {
        return this->PORT;
    }

    // ------------ Helpers ---------------
    sockaddr_in getSockAddr() const {
        sockaddr_in ADDR{};

        ADDR.sin_family = AF_INET;
        ADDR.sin_port = htons(PORT);

        inet_pton(
            AF_INET,
            this->IP_ADDRESS.c_str(),
            &ADDR.sin_addr);

        return ADDR;
    }
    socklen_t getSockAddrLen() const {
        return sizeof(sockaddr_in);
    }
}: