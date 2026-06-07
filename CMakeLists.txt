cmake_minimum_required(VERSION 3.20)

project(
    TCPProject
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(
    -Wall
    -Wextra
    -Wpedantic
)

file(GLOB_RECURSE TCP_CORE_SOURCES
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_library(tcp_core STATIC
    ${TCP_CORE_SOURCES}
)

target_include_directories(tcp_core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(client
    apps/client_main.cpp
)

target_link_libraries(client
    PRIVATE
        tcp_core
)

add_executable(server
    apps/server_main.cpp
)

target_link_libraries(server
    PRIVATE
        tcp_core
)