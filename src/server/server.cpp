#include "server.hpp"

#include <unistd.h>

#include "connection.hpp"

int main() {
    Server server("127.0.0.1", 8080);

    server.createListenSocket();
    server.bindListenSocket();
    server.startListening();

    while (true) {
        // Accepting Clients
        Connection conn = server.acceptConnection();

        if (!conn.getSocket().isValid()) {
            std::cerr << "No client connected" << std::endl
                      << "Retrying in 5 seconds..." << std::endl;

            sleep(5);
            continue;
        }

        std::cout << "Client connected" << std::endl;

        // Messaging Client
        while (true) {
            std::string request = conn.receiveData();

            if (
                request == "NOT-EXISTS" ||
                request == "DISCONNECTED" ||
                request == "FAILED") {
                std::cout << "Client Disconnected" << std::endl;
                break;
            }

            cout << "Request: " << request << std::endl;

            std::string reply;
            std::cout << "Enter Reply: ";
            std::getline(std::cin, reply);

            conn.sendData(reply);
        }
    }

    return 0;
}