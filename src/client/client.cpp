#include "client.hpp"

#include <unistd.h>

void Client::run() {
    createClientSocket();

    while (true) {
        // Connecting to Server
        while (!connectToServer()) {
            std::cerr << "could not connect to server" << std::endl
                      << "Retrying in 5 seconds..." << std::endl;

            sleep(5);
        }

        // Messaging Server
        while (true) {
            std::string request;
            std::cout << "Enter Request: ";
            std::getline(std::cin, request);

            getConnection().sendData(request);

            std::string response = getConnection().receiveData();

            if (
                response.empty() ||
                response == "NOT-EXISTS" ||
                response == "DISCONNECTED" ||
                response == "FAILED") {
                std::cout << "Server disonnected" << std::endl;
                break;
            }

            cout << "Response: " << response << std::endl;
        }
    }
}