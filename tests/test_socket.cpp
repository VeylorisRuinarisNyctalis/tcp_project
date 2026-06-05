#include <iostream>

#include "core/socket.hpp"

void testSocketCreation() {
    std::cout << "[Socket Test] Creating socket...\n";

    Socket s;
    s.createSocket();

    if (s.isValid()) {
        std::cout << "✔ Socket creation PASS\n";
    } else {
        std::cout << "✘ Socket creation FAIL\n";
    }

    std::cout << "[Socket Test] Closing socket...\n";
    s.closeSocket();

    if (!s.isValid()) {
        std::cout << "✔ Socket close PASS\n";
    } else {
        std::cout << "✘ Socket close FAIL\n";
    }
}

int main() {
    testSocketCreation();
    return 0;
}