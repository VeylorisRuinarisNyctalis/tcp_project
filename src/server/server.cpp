#include "server/server.hpp"

#include <thread>
#include <unistd.h>

#include "core/network/connection.hpp"

void Server::handleClient(Connection conn) {
    std::cout
        << "[Client "
        << conn.getID()
        << "] Handler thread started."
        << std::endl;

    while (true) {
        ReceiveResult result =
            conn.receiveData();

        if (result.status != ReceiveStatus::Success) {
            std::cout
                << "[Client "
                << conn.getID()
                << "] Disconnected."
                << std::endl;

            break;
        }

        std::string request =
            result.data;

        std::cout
            << "[Client "
            << conn.getID()
            << "] Request: "
            << request
            << std::endl;

        std::string reply =
            "Echo: " + request;

        if (!conn.sendData(reply)) {
            std::cerr
                << "[Client "
                << conn.getID()
                << "] Failed to send reply."
                << std::endl;

            break;
        }

        std::cout
            << "[Client "
            << conn.getID()
            << "] Reply sent."
            << std::endl;
    }

    std::cout
        << "[Client "
        << conn.getID()
        << "] Handler thread finished."
        << std::endl;
}

void Server::run() {
    std::cout
        << "[Server] Starting server..."
        << std::endl;

    if (!createListenSocket()) {
        std::cerr
            << "[Server] Failed to create listening socket."
            << std::endl;
        return;
    }

    if (!bindListenSocket()) {
        std::cerr
            << "[Server] Failed to bind listening socket."
            << std::endl;
        return;
    }

    if (!startListening()) {
        std::cerr
            << "[Server] Failed to start listening."
            << std::endl;
        return;
    }

    std::cout
        << "[Server] Entering accept loop."
        << std::endl;

    while (true) {
        Connection conn =
            acceptConnection();

        if (!conn.getSocket().isValid()) {
            std::cerr
                << "[Server] acceptConnection() failed."
                << std::endl;

            std::cout
                << "[Server] Retrying in 5 seconds..."
                << std::endl;

            sleep(5);
            continue;
        }

        std::cout
            << "[Server] Client "
            << conn.getID()
            << " connected."
            << std::endl;

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