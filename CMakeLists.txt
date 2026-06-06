cmake_minimum_required(VERSION 3.20)

project(TCPProject)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

# Shared library
add_library(tcp_core
    src/core/tcp_protocol.cpp
    src/client/client.cpp
    src/server/server.cpp
)

# Client executable
add_executable(client
    apps/client_main.cpp
)

target_link_libraries(client tcp_core)

# Server executable
add_executable(server
    apps/server_main.cpp
)

target_link_libraries(server tcp_core)