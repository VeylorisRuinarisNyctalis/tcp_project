#include <iostream>

#include "core/connection.hpp"
#include "core/endpoint.hpp"

void testConnectionBasic() {
    std::cout << "[Connection Test] Starting...\n";

    Endpoint serverEp("127.0.0.1", 8080);

    Connection conn(serverEp);

    std::string msg = "hello server";

    std::cout << "Sending message: " << msg << "\n";

    bool sent = conn.sendData(msg);

    if (sent) {
        std::cout << "✔ sendData PASS\n";
    } else {
        std::cout << "✘ sendData FAIL (expected if no server running)\n";
    }

    std::string response = conn.receiveData();

    std::cout << "Received: " << response << "\n";
}

int main() {
    testConnectionBasic();
    return 0;
}