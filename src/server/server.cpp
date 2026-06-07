#include "server/server.hpp"

#include <thread>

#include "core/network/connection.hpp"

void Server::handleClient(Connection conn) {
    while (true) {
        std::string request = conn.receiveData();

        if (request == "NOT-EXISTS" || request == "DISCONNECTED" ||
            request == "FAILED") {
            std::cout << "Client Disconnected" << std::endl;
            break;
        }

        std::cout << "Request: " << request << std::endl;

        std::string reply;
        std::cout << "Enter Reply: ";
        std::getline(std::cin, reply);

        conn.sendData(reply);
    }
}

void Server::run() {
    createListenSocket();
    bindListenSocket();
    startListening();

    while (true) {
        // Accepting Clients
        Connection conn = acceptConnection();

        if (!conn.getSocket().isValid()) {
            std::cerr << "No client connected" << std::endl
                      << "Retrying in 5 seconds..." << std::endl;

            sleep(5);
            continue;
        }

        std::cout << "Client connected" << std::endl;

        // Messaging Client
        std::thread(&Server::handleClient, this, std::move(conn)).detach();
    }
}