#include "server.hpp"

#include "connection.hpp"

int main() {
    Server server("127.0.0.1", 9090);

    server.createListenSocket();
    server.bindListenSocket();
    server.startListening();

    Connection conn = server.acceptConnection();

    // Messaging
    while (true) {
        std::string request = conn.receiveData();
        cout << "Request: " << request << std::endl;

        std::string reply;
        std::cout << "Enter Reply: ";
        std::getline(std::cin, reply);

        conn.sendData(reply);
    }

    return 0;
}