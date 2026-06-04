#include <iostream>
#include <string>

#include "tcp_protocol.hpp"

using namespace std;

class Client : public TCP_Protocol {
public:
    Client() : TCP_Protocol() {}
    Client(const string& IP_ADDRESS, const int PORT) : TCP_Protocol(IP_ADDRESS, PORT) {}

    ~Client() {
        closeSocket(SOCKET_FD);
    }

    bool connectToServer() {
        sockaddr_in SERVER_ADDR{};

        SERVER_ADDR.sin_family = AF_INET;
        SERVER_ADDR.sin_port = htons(PORT);
        SERVER_ADDR.sin_addr.s_addr = inet_addr(IP_ADDRESS.c_str());

        if (
            connect(
                SOCKET_FD,
                reinterpret_cast<sockaddr*>(&SERVER_ADDR),
                sizeof(SERVER_ADDR)) < 0) {
            cerr << "Failed to connect to Server!\n";
            return false;
        }

        cout << "Connected to Server Succesfully!\n";
        return true;
    }

    void run() {
        if (!createSocket(SOCKET_FD)) {
            return;
        }
        if (!connectToServer()) {
            return;
        }

        while (true) {
            cout << "Enter Message: ";
            string message;
            getline(cin, message);

            sendData(SOCKET_FD, message);

            string response = receiveData(SOCKET_FD);

            if (!response.empty()) {
                cout << "Response: "
                     << response
                     << std::endl;
            }
        }

        closeSocket(SOCKET_FD);
    }
};

int main() {
    Client client("127.0.0.1", 9090);

    client.run();

    return 0;
}
