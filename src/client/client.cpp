#include "client.hpp"

#include <unistd.h>

int main() {
    Client client("127.0.0.1", 8080);

    client.createClientSocket();

    while (true) {
        // Connecting to Server
        while (!client.connectToServer()) {
            std::cerr << "could not connect to server" << std::endl
                      << "Retrying in 5 seconds..." << std::endl;

            sleep(5);
        }

        // Messaging Server
        while (true) {
            std::string request;
            std::cout << "Enter Request: ";
            std::getline(std::cin, request);

            client.getConnection().sendData(request);

            std::string response = client.getConnection().receiveData();

            if (
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

return 0;
}