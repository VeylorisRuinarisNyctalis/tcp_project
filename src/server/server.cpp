#include "server/server.hpp"

#include <thread>
#include <unistd.h>

#include "core/network/connection.hpp"

void Server::handleClient(Connection conn) {
    std::cout
        << "[Server] Client handler thread started."
        << std::endl;

    while (true) {
        std::string request = conn.receiveData();

        if (
            request == "NOT-EXISTS" ||
            request == "DISCONNECTED" ||
            request == "FAILED") {
            std::cout
                << "[Server] Client disconnected."
                << std::endl;

            break;
        }

        std::cout
            << "[Server] Request received: "
            << request
            << std::endl;

        std::string reply= "Echo: " + request;

        if (!conn.sendData(reply)) {
            std::cerr
                << "[Server] Failed to send reply."
                << std::endl;

            break;
        }

        std::cout
            << "[Server] Reply sent."
            << std::endl;
    }

    std::cout
        << "[Server] Client handler thread finished."
        << std::endl;
}

void Server::run() {
    std::cout
        << "[Server] Starting server..."
        << std::endl;

    createListenSocket();
    bindListenSocket();
    startListening();

    std::cout
        << "[Server] Entering accept loop."
        << std::endl;

    while (true) {
        // Accepting Clients
        Connection conn = acceptConnection();

        if (!conn.getSocket().isValid()) {
            std::cerr
                << "[Server] No client connected."
                << std::endl;

            std::cout
                << "[Server] Retrying in 5 seconds..."
                << std::endl;

            sleep(5);
            continue;
        }

        std::cout
            << "[Server] Client connected."
            << std::endl;

        // Messaging Client
        std::thread(
            &Server::handleClient,
            this,
            std::move(conn))
            .detach();

        std::cout
            << "[Server] Client thread detached."
            << std::endl;
    }
}