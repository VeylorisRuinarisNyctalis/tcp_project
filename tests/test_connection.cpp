#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cassert>
#include <string>

#include "core/network/connection.hpp"

sockaddr_in makeEndpointAddress() {
    sockaddr_in addr{};

    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);

    int converted =
        inet_pton(
            AF_INET,
            "127.0.0.1",
            &addr.sin_addr);

    assert(converted == 1);

    return addr;
}

int main() {
    Connection invalid;

    assert(!invalid.sendData("hello"));

    ReceiveResult invalidReceive =
        invalid.receiveData();

    assert(invalidReceive.status == ReceiveStatus::InvalidSocket);
    assert(invalidReceive.data.empty());

    int sockets[2];

    assert(socketpair(AF_UNIX, SOCK_STREAM, 0, sockets) == 0);

    Connection conn(
        42,
        sockets[0],
        makeEndpointAddress());

    assert(conn.getID() == 42);
    assert(conn.getSocket().isValid());

    const std::string incoming = "ping";

    ssize_t sent =
        send(
            sockets[1],
            incoming.data(),
            incoming.size(),
            0);

    assert(sent == static_cast<ssize_t>(incoming.size()));

    ReceiveResult received =
        conn.receiveData();

    assert(received.status == ReceiveStatus::Success);
    assert(received.data == incoming);

    const std::string outgoing = "pong";

    assert(conn.sendData(outgoing));

    char buffer[16];

    ssize_t bytes =
        recv(
            sockets[1],
            buffer,
            sizeof(buffer),
            0);

    assert(bytes == static_cast<ssize_t>(outgoing.size()));
    assert(std::string(buffer, bytes) == outgoing);

    close(sockets[1]);

    return 0;
}
