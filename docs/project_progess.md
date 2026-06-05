# TCP Client/Server Project

## Project Goal

Build a modular TCP Client/Server networking framework in C++ using IPv4 sockets and localhost communication (`127.0.0.1`).

The project is designed not only to understand the complete TCP socket lifecycle but also to establish a scalable architecture capable of supporting:

- Multiple Clients
- Threading and Concurrency
- Connection Management
- Packet-Based Communication
- Heartbeat Monitoring
- File Transfer
- Chat Applications

---

# Current Project Structure

```text
project/
│
├── socket.hpp
├── endpoint.hpp
├── connection.hpp
├── server.hpp
└── client.hpp
```

---

# Current Architecture

## Socket

Encapsulates socket descriptor ownership and lifecycle management.

### Responsibilities

- Socket creation
- Socket cleanup
- Socket validation
- File descriptor access

### Public Interface

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

### Stores

```text
IP Address
Port
```

### Responsibilities

- Store endpoint information
- Validate IPv4 addresses
- Convert endpoint data into `sockaddr_in`
- Construct endpoint information from received socket addresses

### Public Interface

```cpp
getIPAddress()
getPort()

getSockAddr()

isValidIP()
```

---

## Connection

Represents an active TCP connection.

### Composition

```text
Connection
│
├── Socket
└── Endpoint
```

### Responsibilities

- Store connection socket
- Store remote endpoint information
- Send data
- Receive data

### Public Interface

```cpp
sendData()
receiveData()

getSocket()
getEndpoint()
```

---

## Server

Represents the listening side of the TCP application.

### Composition

```text
Server
│
├── Listening Socket
└── Endpoint
```

### Responsibilities

- Create listening socket
- Bind socket
- Listen for incoming connections
- Accept client connections

### Public Interface

```cpp
createListenSocket()
bindListenSocket()

startListening()

acceptConnection()
```

### Connection Acceptance Flow

```text
Listening Socket
        │
        ▼
    accept()
        │
        ▼
   Connection
      │
      ├── Socket
      └── Endpoint
```

---

## Client

Represents the connecting side of the TCP application.

### Composition

```text
Client
│
└── Connection
    │
    ├── Socket
    └── Endpoint
```

### Responsibilities

- Create client socket
- Connect to server

### Public Interface

```cpp
createClientSocket()

connectToServer()
```

---

# TCP Socket Lifecycle

## Server Side

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

---

## Client Side

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

# Current Communication Flow

```text
Client
    │
    ▼
connect()
    │
    ▼
Server Listening Socket
    │
    ▼
accept()
    │
    ▼
Connection Object
    │
    ├── Socket
    └── Endpoint
    │
    ▼
sendData()/receiveData()
```

---

# Current Implementation Status

## Socket

```text
✓ createSocket()
✓ closeSocket()
✓ getFD()
✓ setFD()
✓ isValid()
✓ Automatic cleanup through destructor
```

---

## Endpoint

```text
✓ IP Address Storage
✓ Port Storage
✓ IPv4 Validation
✓ sockaddr_in Conversion
✓ Endpoint Construction from sockaddr_in
```

---

## Connection

```text
✓ Socket Ownership
✓ Endpoint Ownership
✓ sendData()
✓ receiveData()
```

---

## Server

```text
✓ createListenSocket()
✓ bindListenSocket()
✓ startListening()
✓ acceptConnection()
```

---

## Client

```text
✓ createClientSocket()
✓ connectToServer()
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
│
├── apps/
│   ├── server_main.cpp
│   └── client_main.cpp
│
├── tests/
│
├── docs/
│
├── build/
│
├── Makefile
│
└── README.md
```

---

# Why This Architecture?

Future milestones will significantly increase project complexity:

```text
Multiple Clients
        ↓
Threading
        ↓
Client Manager
        ↓
Packet Protocol
        ↓
Heartbeat Detection
        ↓
File Transfer
        ↓
Chat System
```

The architecture separates:

- Socket Management
- Endpoint Management
- Connection Handling
- Server Logic
- Client Logic

allowing the project to scale without concentrating all networking functionality into a single class.

---

# Upcoming Mentor Requirements

The following milestones represent the next set of requirements and enhancements for the TCP Client/Server framework.

## 1. Connection Failure Detection

The client should be capable of detecting connection failures after a successful connection has been established, including:

```text
Server crash
Server disconnect
Server timeout
```

---

## 2. Client Failure Detection

The server should be capable of detecting client-side failures after a successful connection has been established, including:

```text
Client crash
Client disconnect
Client timeout
```

---

## 3. Multiple Message Exchange

Support continuous communication between the client and server, allowing multiple messages to be sent and received during a single connection session.

---

## 4. Multiple Client Support

Extend the server architecture to handle multiple simultaneous client connections.

---

## 5. std::thread Concurrency

Introduce multithreading using `std::thread` to enable concurrent client handling and improve scalability.

---

## 6. Heartbeat / Failure Monitoring

Implement heartbeat mechanisms to actively monitor connection health and detect unresponsive peers.

---

## 7. Persistent Server and Client Processes

Design the server and client applications to run continuously until explicitly terminated, rather than exiting after a single interaction.

---

## 8. Blocking and Non-Blocking I/O

Explore and implement both blocking and non-blocking socket operations, including buffer management considerations and their impact on application behavior.

# Current Status Summary

Successfully implemented and refactored a TCP Client/Server application using localhost communication.

Current architecture separates:

```text
Socket
    ↓
Endpoint
    ↓
Connection
    ↓
Client / Server
```

and provides a foundation for future multi-client, threaded, and production-style networking features.
