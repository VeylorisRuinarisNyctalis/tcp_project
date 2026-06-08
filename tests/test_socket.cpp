#include <cassert>

#include "core/network/socket.hpp"

int main() {
    Socket socket;

    assert(!socket.isValid());

    assert(socket.createSocket());
    assert(socket.isValid());

    int fd = socket.getFD();

    assert(fd != -1);
    assert(!socket.createSocket());
    assert(socket.getFD() == fd);

    assert(socket.closeSocket());
    assert(!socket.isValid());

    assert(socket.closeSocket());

    return 0;
}
