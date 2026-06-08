#include <arpa/inet.h>
#include <sys/socket.h>

#include <cassert>
#include <functional>
#include <future>
#include <thread>

#include "client/client.hpp"
#include "server/server.hpp"

uint16_t getBoundPort(const Server& server) {
    sockaddr_in addr{};
    socklen_t addrLen = sizeof(addr);

    int result =
        getsockname(
            server.getListeningSocket().getFD(),
            reinterpret_cast<sockaddr*>(&addr),
            &addrLen);

    assert(result == 0);

    return ntohs(addr.sin_port);
}

void runServer(std::promise<uint16_t>& portPromise) {
    Server server("127.0.0.1", 0);

    assert(server.createListenSocket());
    assert(server.bindListenSocket());
    assert(server.startListening());

    portPromise.set_value(getBoundPort(server));

    auto conn = server.acceptConnection();

    assert(conn.getSocket().isValid());

    ReceiveResult msg =
        conn.receiveData();

    assert(msg.status == ReceiveStatus::Success);
    assert(msg.data == "hello server");

    assert(conn.sendData("hello client"));
}

void runClient(uint16_t port) {
    Client client("127.0.0.1", port);

    assert(client.createClientSocket());
    assert(client.connectToServer());
    assert(client.getConnection().sendData("hello server"));

    ReceiveResult reply =
        client.getConnection().receiveData();

    assert(reply.status == ReceiveStatus::Success);
    assert(reply.data == "hello client");
}

int main() {
    std::promise<uint16_t> portPromise;
    std::future<uint16_t> portFuture =
        portPromise.get_future();

    std::thread serverThread(
        runServer,
        std::ref(portPromise));

    uint16_t port =
        portFuture.get();

    std::thread clientThread(
        runClient,
        port);

    clientThread.join();
    serverThread.join();

    return 0;
}
