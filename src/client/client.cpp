#include "client.hpp"

#include <unistd.h>

int main() {
    Client client("127.0.0.1", 8080);

    client.createClientSocket();
    client.connectToServer();

    // Messaging
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
            std::cout << "Server Not Connected" << std::endl;
            break;
        }
    }

    cout << "Response: " << response << std::endl;
}

return 0;
}