#include "client.hpp"

int main() {
    Client client("127.0.0.1", 9090);

    client.createClientSocket();
    client.connectToServer();

    // Messaging
    std::string request;
    std::cout << "Enter Request: ";
    getline(std::cin, request);

    client.getConnection().sendData(request);

    std::string response = client.getConnection().receiveData();

    cout << "Response: " << response << std::endl;

    return 0;
}