#include <iostream>

#include "core/endpoint.hpp"

void testEndpoint() {
    std::cout << "[Endpoint Test] Creating endpoint...\n";

    Endpoint ep("127.0.0.1", 8080);

    if (ep.isValidIP()) {
        std::cout << "✔ IP validation PASS\n";
    } else {
        std::cout << "✘ IP validation FAIL\n";
    }

    std::cout << "IP: " << ep.getIPAddress() << "\n";
    std::cout << "Port: " << ep.getPort() << "\n";

    sockaddr_in addr = ep.getSockAddr();
    std::cout << "✔ sockaddr_in conversion OK\n";
}

int main() {
    testEndpoint();
    return 0;
}