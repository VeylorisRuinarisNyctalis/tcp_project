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
Endpoint(const sockaddr_in& addr)

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
├── ID
├── Socket
└── Endpoint
```

Responsibilities:

- Own socket + endpoint
- Track connection identity
- Send data
- Receive data
- Report receive status

Interface:

```cpp
setID()

getID()

sendData()
receiveData() -> ReceiveResult

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
- Handle each client in a detached worker thread
- Echo requests back to connected clients

Interface:

```cpp
createListenSocket()
bindListenSocket()

startListening()
acceptConnection()
handleClient()
run()
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
- Retry connection after failure
- Run the interactive request/response loop

Interface:

```cpp
createClientSocket()
connectToServer()
run()
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
Detached handler thread
    ↓
Connection (ID + Socket + Endpoint)
    ↓
sendData() / receiveData() -> ReceiveResult
```

---

# Current Status

Implemented:

```text
✓ Socket abstraction (create/close/FD ownership, move-only semantics)
✓ Endpoint abstraction (IPv4 validation, sockaddr_in conversion, raw-addr constructor)
✓ Connection abstraction (ID + socket/endpoint ownership, sendData/receiveData via ReceiveResult)
✓ Server runtime (bind/listen/accept loop with detached per-client echo handlers)
✓ Client runtime (socket creation, reconnect loop, stdin request/response cycle)
✓ CMake build targets for `tcp_core`, `server`, `client`, and test executables
✓ CTest coverage for socket lifecycle, endpoint conversion, connection I/O, and client/server exchange
```

### 🧠 Latest Updates Integrated

The system now reflects concrete runtime behavior rather than just design sketches:

- `Socket` now explicitly supports:
    - constructor overloads
    - move semantics
    - FD setter/getter
    - lifecycle + validation

- `Endpoint` now supports:
    - `sockaddr_in` conversion constructor
    - structured IP/port access
    - validation layer for IPv4 correctness

- `Connection` now includes:
    - dual constructors (IP-based + raw socket-based)
    - connection IDs and ownership accessors
    - bidirectional data API (`sendData`, `receiveData`)
    - `ReceiveResult` status reporting

- Runtime behavior now clearly defined:
    - threaded per-client server handling
    - interactive client request/response loop
    - reconnect-on-failure client flow with socket recreation
    - reusable server bind setup via `SO_REUSEADDR`
    - CTest-backed verification for core networking behavior

### Client Loop Behavior

```text
connect retry loop
    ↓
continuous request-response cycle
    ↓
socket close on disconnect
    ↓
return to connection loop
```

### Server Loop Behavior

```text
accept loop
    ↓
detached per-client handler thread
    ↓
request processing + Echo: reply cycle
```

---

# Future Architecture (Target)

The current codebase already has the base networking layers in place; the target below is the next split for packet framing and shared utilities.

```text
tcp-project/
│
├── CMakeLists.txt
├── include/
│   │
│   ├── core/
│   │   ├── network/
│   │   │   ├── socket.hpp
│   │   │   ├── endpoint.hpp
│   │   │   └── connection.hpp
│   │   └── protocol/
│   │       └── receive_result.hpp
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
└── docs/
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
- Protocol framing
- Client coordination

Multiple clients and detached worker threads are already in place; the remaining roadmap centers on coordinated multi-client state, packet framing, heartbeats, transfer features, and richer messaging.

---

# Upcoming Requirements

1. Connection failure detection and recovery
    - server crash
    - server disconnect
    - server timeout

2. Client failure detection and recovery
    - client crash
    - client disconnect
    - client timeout

3. Packet framing for continuous message exchange

4. Dedicated client manager / shared connection registry

5. Heartbeat monitoring

6. File transfer support

7. Blocking vs non-blocking I/O strategy

8. Persistent server/client process supervision

---

# Summary

A modular TCP Client/Server framework built with CMake, with a header-based socket/endpoint/connection layer, a threaded echo-style server, an interactive reconnecting client, and CTest coverage for the current networking flow. The remaining work is to add packet framing, heartbeat and transfer features, and coordinated client management.
