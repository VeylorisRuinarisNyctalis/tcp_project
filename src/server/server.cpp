#include "server.hpp"

#include "connection.hpp"

int main() {
    Server server("127.0.0.1", 9090);

    server.createListenSocket();
    server.bindListenSocket();
    server.startListening();

    Connection CONNECTION = server.acceptConnection();

    // Messaging
    std::string request = CONNECTION.receiveData();
    cout << "Request: " << request << std::endl;

    std::string reply;
    std::cout << "Enter Reply: ";
    getline(std::cin, reply);

    CONNECTION.sendData(reply);

    return 0;
}