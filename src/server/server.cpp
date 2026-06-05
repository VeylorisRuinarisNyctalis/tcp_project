#include "server.hpp"

#include <unistd.h>

#include "connection.hpp"

int main() {
    Server server("127.0.0.1", 8080);

    server.createListenSocket();
    server.bindListenSocket();
    server.startListening();

    Connection conn = server.acceptConnection();

    // Messaging
    while (true) {
        std::string request = conn.receiveData();

        if (
            request == "NOT-EXISTS" ||
            request == "DISCONNECTED" ||
            request == "FAILED") {
            std::cout << "Client Not Connected" << std::endl;
            break;
        }

        cout << "Request: " << request << std::endl;

        std::string reply;
        std::cout << "Enter Reply: ";
        std::getline(std::cin, reply);

        conn.sendData(reply);
    }

    return 0;
}