#include "client/client.hpp"

#include <unistd.h>

void Client::run() {
    std::cout
        << "[Client] Starting client..."
        << std::endl;

    while (true) {
        if (!getConnection().getSocket().isValid()) {
            if (!createClientSocket()) {
                std::cerr
                    << "[Client] Failed to create socket."
                    << std::endl;

                std::cout
                    << "[Client] Retrying in 5 seconds..."
                    << std::endl;

                sleep(5);
                continue;
            }
        }

        std::cout
            << "[Client] Entering connection loop."
            << std::endl;

        while (!connectToServer()) {
            std::cerr
                << "[Client] Could not connect to server."
                << std::endl;

            getConnection().getSocket().closeSocket();

            std::cout
                << "[Client] Retrying in 5 seconds..."
                << std::endl;

            sleep(5);

            if (!createClientSocket()) {
                std::cerr
                    << "[Client] Failed to create socket."
                    << std::endl;

                std::cout
                    << "[Client] Retrying in 5 seconds..."
                    << std::endl;

                sleep(5);
            }
        }

        std::cout
            << "[Client] Connected. Entering messaging loop."
            << std::endl;

        while (true) {
            std::string request;

            std::cout
                << "Enter Request: ";

            std::getline(std::cin, request);

            if (!std::cin) {
                std::cerr
                    << "[Client] Input stream error."
                    << std::endl;

                return;
            }

            if (request.empty()) {
                std::cout
                    << "[Client] Empty request entered."
                    << std::endl;

                continue;
            }

            if (!getConnection().sendData(request)) {
                std::cerr
                    << "[Client] Failed to send request."
                    << std::endl;

                break;
            }

            ReceiveResult response =
                getConnection().receiveData();

            if (response.status != ReceiveStatus::Success) {
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
                << response.data
                << std::endl;
        }

        std::cout
            << "[Client] Connection lost."
            << std::endl;

        getConnection().getSocket().closeSocket();

        std::cout
            << "[Client] Returning to connection loop."
            << std::endl;
    }
}
