#include <chrono>
#include <iostream>
#include <thread>

#include "client/client.hpp"
#include "server/server.hpp"

void runServer() {
    Server server(8080);

    server.createListenSocket();
    server.bindListenSocket();
    server.startListening();

    auto conn = server.acceptConnection();

    std::string msg = conn.receiveData();
    std::cout << "[Server] Received: " << msg << "\n";

    conn.sendData("hello client");
}

void runClient() {
    std::this_thread::sleep_for(std::chrono::seconds(1));

    Client client("127.0.0.1", 8080);
    client.createClientSocket();
    client.connectToServer();

    client.getConnection().sendData("hello server");

    std::string reply = client.getConnection().receiveData();
    std::cout << "[Client] Received: " << reply << "\n";
}

int main() {
    std::thread serverThread(runServer);
    std::thread clientThread(runClient);

    serverThread.join();
    clientThread.join();

    return 0;
}