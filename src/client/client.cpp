#include "client/client.hpp"

#include <unistd.h>

void Client::run() {
    std::cout
        << "[Client] Starting client..."
        << std::endl;

    createClientSocket();

    while (true) {
        std::cout
            << "[Client] Entering connection loop."
            << std::endl;

        // Connecting to Server
        while (!connectToServer()) {
            std::cerr
                << "[Client] Could not connect to server."
                << std::endl;

            std::cout
                << "[Client] Retrying in 5 seconds..."
                << std::endl;

            sleep(5);
        }

        std::cout
            << "[Client] Connected. Entering messaging loop."
            << std::endl;

        // Messaging Server
        while (true) {
            std::string request;

            std::cout << "Enter Request: ";
            std::getline(std::cin, request);

            if (request.empty()) {
                std::cout
                    << "[Client] Empty request entered."
                    << std::endl;
            }

            if (!getConnection().sendData(request)) {
                std::cerr
                    << "[Client] Failed to send request."
                    << std::endl;
                break;
            }

            std::string response =
                getConnection().receiveData();

            if (
                response.empty() ||
                response == "NOT-EXISTS" ||
                response == "DISCONNECTED" ||
                response == "FAILED") {
                std::cout
                    << "[Client] Server disconnected."
                    << std::endl;

                break;
            }

            std::cout
                << "[Client] Server response received."
                << std::endl;

            std::cout
                << "Response: "
                << response
                << std::endl;
        }

        std::cout
            << "[Client] Returning to connection loop."
            << std::endl;
    }
}