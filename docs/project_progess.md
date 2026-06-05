# TCP Client/Server Project

## Goal

Build a modular TCP Client/Server networking framework in C++ using IPv4 sockets and localhost (`127.0.0.1`).

Designed to progressively support:

- Multiple clients
- Threading & concurrency
- Connection management
- Packet-based communication
- Heartbeat monitoring
- File transfer
- Chat applications

---

# Current Architecture

```text
Socket
    ↓
Endpoint
    ↓
Connection
    ↓
Client / Server
```

---

## Socket

Encapsulates socket file descriptor lifecycle.

Responsibilities:

- Create socket
- Close socket
- Validate socket
- Manage FD ownership

Interface:

```cpp
createSocket()
closeSocket()

getFD()
setFD()

isValid()
```

---

## Endpoint

Represents a network endpoint.

Stores:

```text
IP Address
Port
```

Responsibilities:

- IPv4 validation
- sockaddr_in conversion
- Construct from sockaddr_in
- Provide endpoint metadata

Interface:

```cpp
getIPAddress()
getPort()

getSockAddr()

isValidIP()
```

---

## Connection

Represents an active TCP connection.

```text
Connection
├── Socket
└── Endpoint
```

Responsibilities:

- Own socket + endpoint
- Send data
- Receive data

Interface:

```cpp
sendData()
receiveData()

getSocket()
getEndpoint()
```

---

## Server

Listening side of the system.

```text
Server
├── Listening Socket
└── Endpoint
```

Responsibilities:

- Create/bind/listen socket
- Accept incoming connections

Interface:

```cpp
createListenSocket()
bindListenSocket()

startListening()
acceptConnection()
```

---

## Client

Connecting side of the system.

```text
Client
└── Connection
```

Responsibilities:

- Create client socket
- Connect to server

Interface:

```cpp
createClientSocket()
connectToServer()
```

---

# TCP Lifecycle

## Server

```text
socket()
    ↓
bind()
    ↓
listen()
    ↓
accept()
    ↓
send()/recv()
    ↓
close()
```

## Client

```text
socket()
    ↓
connect()
    ↓
send()/recv()
    ↓
close()
```

---

# Communication Flow

```text
Client
    ↓
connect()
    ↓
Server Listener
    ↓
accept()
    ↓
Connection (Socket + Endpoint)
    ↓
sendData() / receiveData()
```

---

# Current Status

Implemented:

```text
✓ Socket abstraction (create/close/FD management)
✓ Endpoint abstraction (IPv4 + sockaddr_in conversion)
✓ Connection abstraction (send/receive over socket)
✓ Server (listen + accept loop with per-client handling flow)
✓ Client (connect + retry loop + request/response loop skeleton)
```

### 🧠 Latest Updates Integrated

The system now reflects **fully defined internal class APIs and runtime loops**:

- `Socket` now explicitly supports:
    - constructor overloads
    - FD setter/getter
    - lifecycle + validation

- `Endpoint` now supports:
    - `sockaddr_in` conversion constructor
    - structured IP/port access
    - validation layer for IPv4 correctness

- `Connection` now includes:
    - dual constructors (IP-based + raw socket-based)
    - bidirectional data API (`sendData`, `receiveData`)
    - full ownership accessors

- Runtime behavior now clearly defined:

### Client Loop Behavior

```text
connect retry loop
    ↓
continuous request-response cycle
    ↓
disconnect handling (planned)
```

### Server Loop Behavior

```text
accept loop
    ↓
per-client connection handling loop
    ↓
request processing + reply cycle
```

---

# Future Architecture (Target)

```text
tcp-project/
│
├── include/
│   │
│   ├── core/
│   │   ├── socket.hpp
│   │   ├── endpoint.hpp
│   │   ├── connection.hpp
│   │   └── packet.hpp
│   │
│   ├── server/
│   │   ├── server.hpp
│   │   └── client_manager.hpp
│   │
│   ├── client/
│   │   └── client.hpp
│   │
│   └── utils/
│       ├── logger.hpp
│       └── helpers.hpp
│
├── src/
│   ├── client/
│   │   └── client.cpp
│   ├── server/
│   │   └── server.cpp
│   │
│   └── core/network/
│       ├── socket.cpp
│       ├── endpoint.cpp
│       └── connection.cpp
│
├── apps/
│   ├── server_main.cpp
│   └── client_main.cpp
│
├── tests/
├── docs/
├── build/
├── Makefile
└── README.md
```

---

# Scaling Roadmap

```text
Multiple Clients
    ↓
std::thread
    ↓
Client Manager
    ↓
Packet Protocol
    ↓
Heartbeat System
    ↓
File Transfer
    ↓
Chat System
```

Architecture separation goals:

- Socket management
- Endpoint management
- Connection handling
- Server logic
- Client logic

This avoids a monolithic networking design and keeps future concurrency safe.

---

# Upcoming Requirements

1. Connection failure detection
    - server crash
    - server disconnect
    - server timeout

2. Client failure detection
    - client crash
    - client disconnect
    - client timeout

3. Continuous message exchange

4. Multi-client support

5. std::thread concurrency

6. Heartbeat monitoring

7. Persistent server/client processes

8. Blocking vs non-blocking I/O

---

# Summary

A modular TCP Client/Server framework with a clean separation of networking concerns, now upgraded with **explicit class-level design definitions and runtime loop architecture**, preparing the system for multi-client concurrency and protocol-level expansion.
