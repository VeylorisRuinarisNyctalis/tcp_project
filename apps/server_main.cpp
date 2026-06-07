#include "server/server.hpp"

int main() {
    Server server("127.0.0.1", 8080);

    server.run();

    return 0;
}
