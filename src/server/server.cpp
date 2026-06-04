#include <cerrno>
#include <cstring>
#include <iostream>
#include <string>

#include "tcp_protocol.hpp"

using namespace std;

class Server : public TCP_Protocol {
    int CLIENT_SOCKET_FD;

public:
    // Constructor & Destructor
    Server() : TCP_Protocol(), CLIENT_SOCKET_FD(-1) {}
    Server(const string& IP_ADDRESS, const int PORT) : TCP_Protocol(IP_ADDRESS, PORT), CLIENT_SOCKET_FD(-1) {}

    ~Server() {
        closeSocket(CLIENT_SOCKET_FD);
        closeSocket(SOCKET_FD);
    }

    bool bindSocket() {
        cout << "SOCKET_FD: "
             << SOCKET_FD
             << std::endl;

        sockaddr_in SERVER_ADDR{};

        SERVER_ADDR.sin_family = AF_INET;
        SERVER_ADDR.sin_port = htons(PORT);
        SERVER_ADDR.sin_addr.s_addr = inet_addr(IP_ADDRESS.c_str());

        int result = bind(
            SOCKET_FD,
            reinterpret_cast<sockaddr*>(&SERVER_ADDR),
            sizeof(SERVER_ADDR));

        if (result < 0) {
            std::cerr << "Failed to bind Successfully!\n";
            std::cerr << strerror(errno) << std::endl;
            return false;
        }

        cout << "Socket bound Successfully\n";
        return true;
    }

    bool startListening(int backlog = 5) {
        int result = listen(
            SOCKET_FD,
            backlog);

        if (result < 0) {
            std::cerr << "Failed to start listening!\n";
            return false;
        }

        cout << "Server is listening on "
             << IP_ADDRESS
             << ":"
             << PORT
             << std::endl;

        return true;
    }
    int acceptConnection() {
        sockaddr_in CLIENT_ADDR{};

        socklen_t client_len = sizeof(CLIENT_ADDR);

        CLIENT_SOCKET_FD = accept(
            SOCKET_FD,
            reinterpret_cast<sockaddr*>(&CLIENT_ADDR),
            &client_len);

        if (CLIENT_SOCKET_FD < 0) {
            std::cerr << "Failed to accept connection\n";
            return -1;
        }

        cout << "Client connected.\n";
        return CLIENT_SOCKET_FD;
    }

    void run() {
        if (!createSocket(SOCKET_FD)) {
            cout << "1";
            return;
        }
        if (!bindSocket()) {
            cout << "2";
            return;
        }
        if (!startListening()) {
            cout << "3";
            return;
        }
        if (acceptConnection() < 0) {
            cout << "4";
            return;
        }

        while (true) {
            string message = receiveData(CLIENT_SOCKET_FD);

            cout << "Request: "
                 << message
                 << endl;

            cout << "Enter Reply: ";
            string reply;
            getline(cin, reply);

            sendData(CLIENT_SOCKET_FD, reply);
        }

        closeSocket(CLIENT_SOCKET_FD);
    }
};

int main() {
    Server server("127.0.0.1", 9090);

    server.run();

    return 0;
}
