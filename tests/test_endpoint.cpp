#include <arpa/inet.h>

#include <cassert>
#include <stdexcept>
#include <string>

#include "core/network/endpoint.hpp"

int main() {
    Endpoint endpoint("127.0.0.1", 8080);

    assert(endpoint.isValidIP());
    assert(endpoint.getIPAddress() == "127.0.0.1");
    assert(endpoint.getPort() == 8080);

    sockaddr_in addr =
        endpoint.getSockAddr();

    char ip[INET_ADDRSTRLEN];

    inet_ntop(
        AF_INET,
        &addr.sin_addr,
        ip,
        sizeof(ip));

    assert(std::string(ip) == "127.0.0.1");
    assert(ntohs(addr.sin_port) == 8080);

    Endpoint fromAddr(addr);

    assert(fromAddr.getIPAddress() == "127.0.0.1");
    assert(fromAddr.getPort() == 8080);

    Endpoint invalid("invalid-ip", 8080);

    assert(!invalid.isValidIP());

    bool threw = false;

    try {
        invalid.getSockAddr();
    } catch (const std::runtime_error&) {
        threw = true;
    }

    assert(threw);

    return 0;
}
