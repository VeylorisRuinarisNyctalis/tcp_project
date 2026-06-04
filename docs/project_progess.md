# TCP Client/Server Project Progress

## Project Goal

Build a TCP Client/Server application in C++ using the IPv4 loopback address (`127.0.0.1`) to understand the complete TCP socket lifecycle and establish a scalable networking architecture for future expansion.

---

# Initial Project Structure

```text
project/
│
├── server.cpp
├── client.cpp
└── tcp_protocol.hpp
```

---

# Current Architecture

## Base Class

### TCP_Protocol

The common networking functionality was abstracted into a base class:

```cpp
class TCP_Protocol
{
protected:
    string IP_ADDRESS;
    int PORT;
    int SOCKET_FD;

public:
    TCP_Protocol();
    TCP_Protocol(const string& IP, int PORT);

    virtual ~TCP_Protocol();

    bool createSocket();
    bool closeSocket();

    bool sendData(const string& message);
    string receiveData();
};
```

### Responsibilities

- Socket creation
- Socket cleanup
- Sending data
- Receiving data
- Storing common endpoint information:
    - IP Address
    - Port
    - Socket Descriptor

---

## Server Class

```cpp
class Server : public TCP_Protocol
{
private:
    int CLIENT_SOCKET_FD;

public:
    Server();
    Server(const string& IP_ADDRESS, int PORT);
    ~Server();

    bool bindSocket();
    bool startListening(int backlog = 5);
    int acceptConnection();

    void run();
};
```

### Server Responsibilities

- Create socket
- Bind socket
- Listen for connections
- Accept clients
- Exchange messages
- Manage connected client socket

---

## Client Class

```cpp
class Client : public TCP_Protocol
{
public:
    Client();
    Client(const string& IP_ADDRESS, int PORT);
    ~Client();

    bool connectToServer();

    void run();
};
```

### Client Responsibilities

- Create socket
- Connect to server
- Send requests
- Receive responses
- Cleanup resources

---

# Implemented TCP Socket Lifecycle

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

# Concepts Learned

## Socket Descriptor

A socket descriptor is simply an integer returned by:

```cpp
socket(...)
```

It acts as a handle to the underlying operating system socket.

Example:

```cpp
SOCKET_FD = socket(AF_INET, SOCK_STREAM, 0);
```

---

## Listening Socket vs Connected Socket

Important distinction discovered during server implementation:

```text
SOCKET_FD
    → Listening Socket

CLIENT_SOCKET_FD
    → Connected Client Socket
```

After:

```cpp
accept(...)
```

a new socket descriptor is returned.

```text
Listening Socket
      │
      └── Continues Listening

Connected Socket
      │
      └── Handles Communication
```

---

# Major Debugging Sessions

## Bug #1

### Symptom

Server failed to bind correctly.

### Root Cause

Socket descriptor was being passed by value instead of updating the actual member variable.

Example problem:

```cpp
createSocket(...)
```

created a descriptor but the class member was not updated correctly.

### Fix

Updated the actual class member:

```cpp
SOCKET_FD
```

instead of modifying a temporary copy.

### Result

```text
bind()
```

started working correctly.

---

## Bug #2

### Symptom

No output appeared after successful compilation.

### Root Cause

Socket state was not being preserved due to incorrect descriptor handling.

### Fix

Ensured:

```cpp
SOCKET_FD
```

inside the object remained updated throughout the socket lifecycle.

---

## Bug #3

### Symptom

Messages were truncated at spaces.

Example:

```text
Hello World
```

became

```text
Hello
```

### Root Cause

Input extraction was stopping at whitespace.

Likely caused by:

```cpp
cin >>
```

behavior.

### Resolution

Need to use line-based input handling for complete messages.

---

## Bug #4

### Symptom

Compilation errors:

```text
.data()
.size()
.resize()
```

reported as invalid.

### Cause

Incorrect usage and string handling issues.

### Resolution

Adjusted string construction and buffer handling.

---

## Bug #5

### Symptom

Error:

```text
invalid conversion from const void* to void*
```

during receive operations.

### Cause

Improper use of:

```cpp
string::data()
```

for writable buffers.

### Resolution

Updated buffer handling approach to use writable storage correctly.

---

# Development Environment Knowledge

## Vim

Learned:

### Replace All

```vim
:%s/OLD/NEW/g
```

Example:

```vim
:%s/SOCKET/SOCKET_FD/g
```

---

## Multi-File Compilation

Previously:

```bash
g++ file.cpp
```

for single files.

Need to transition toward:

```bash
g++ *.cpp -o app
```

and eventually:

```bash
make
```

using a Makefile.

---

# Future Architecture (Target)

```text
tcp-project/
│
├── include/
│   │
│   ├── core/
│   │   ├── tcp_protocol.hpp
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
│   │
│   ├── core/
│   │   ├── tcp_protocol.cpp
│   │   ├── connection.cpp
│   │   └── packet.cpp
│   │
│   ├── server/
│   │   ├── server.cpp
│   │   └── client_manager.cpp
│   │
│   ├── client/
│   │   └── client.cpp
│   │
│   └── utils/
│       ├── logger.cpp
│       └── helpers.cpp
│
├── apps/
│   │
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

# Why Refactor?

Current structure is acceptable for learning.

However, future milestones will quickly increase complexity:

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

Keeping everything inside:

```text
server.cpp
client.cpp
tcp_protocol.hpp
```

will eventually become difficult to maintain.

The new architecture separates:

- Core networking
- Server logic
- Client logic
- Utility functions
- Applications
- Tests
- Documentation

making the codebase scalable.

---

# Upcoming Mentor Requirements

## Connection Failure Detection

Client should detect:

```text
Server crash
Server disconnect
Server timeout
```

after connection establishment.

---

## Client Failure Detection

Server should detect:

```text
Client crash
Client disconnect
Client timeout
```

after connection establishment.

---

# Roadmap

## Phase 1

```text
✓ Basic TCP Client/Server
```

## Phase 2

```text
→ Multiple Messages
```

## Phase 3

```text
→ Multiple Clients
```

## Phase 4

```text
→ std::thread Concurrency
```

## Phase 5

```text
→ Packet Protocol
```

## Phase 6

```text
→ Heartbeat / Failure Detection
```

## Phase 7

```text
→ File Transfer
```

## Phase 8

```text
→ Chat Application
```

## Phase 9

```text
→ Production-Style Networking Architecture
```

---

# Current Status

## TCP_Protocol

```text
✓ Constructors
✓ Destructor
✓ createSocket()
✓ closeSocket()
✓ sendData()
✓ receiveData()
```

## Server

```text
✓ Constructor
✓ bindSocket()
✓ startListening()
✓ acceptConnection()
✓ run()
```

## Client

```text
✓ Constructor
✓ connectToServer()
✓ run()
```

## End-to-End Communication

```text
Client
    ↓
Request
    ↓
Server
    ↓
Reply
    ↓
Client
```

Successfully compiled, executed, and exchanged messages over TCP using localhost.
