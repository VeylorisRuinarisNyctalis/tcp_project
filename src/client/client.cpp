#include "client.hpp"

int main() {
    Client CLIENT("127.0.0.1", 9090);

    CLIENT.createClientSocket();
    CLIENT.connectToServer();

    // Messaging
    std::string request;
    std::cout << "Enter Request: ";
    getline(std::cin, request);

    CLIENT.getConnection().sendData(request);

    std::string response = CLIENT.getConnection().receiveData();

    cout << "Request: " << request << std::endl;

    return 0;
}