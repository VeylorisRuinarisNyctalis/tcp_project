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

find_package(Threads REQUIRED)

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

target_link_libraries(tcp_core
    PUBLIC
        Threads::Threads
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

enable_testing()

set(TCP_TESTS
    socket
    endpoint
    connection
    server_client
)

foreach(test_name IN LISTS TCP_TESTS)
    set(test_target test_${test_name})

    add_executable(${test_target}
        tests/${test_target}.cpp
    )

    target_link_libraries(${test_target}
        PRIVATE
            tcp_core
    )

    add_test(
        NAME ${test_target}
        COMMAND ${test_target}
    )
endforeach()
