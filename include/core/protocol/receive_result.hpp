#pragma once

#include <string>

enum class ReceiveStatus {
    Success,
    Disconnected,
    Error,
    InvalidSocket
};

struct ReceiveResult {
    ReceiveStatus status;
    std::string data;
};