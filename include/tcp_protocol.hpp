#pragma once


#include<iostream>
#include<string>

#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>


using namespace std;


class TCP_Protocol {
protected:
	 string IP_ADDRESS;	// Stores IP ADDRESS
	 int PORT, SOCKET_FD;	// Stores PORT NO, and SOCKET_FD

public:
	// Constructor
	TCP_Protocol(): IP_ADDRESS("10.23.0.174"), PORT(8080), SOCKET_FD(-1) {}
	TCP_Protocol(const string& IP, const int PORT): IP_ADDRESS(IP), PORT(PORT), SOCKET_FD(-1) {}
	
	// Destructor
	virtual ~TCP_Protocol() {
		closeSocket();
	}

	// Getters & Setters
	// 	Later...

	// -------------- Socket Creating Functions ----------------
	// Returns true, if creates Socket, else false
	bool createSocket() {
		SOCKET_FD= socket(AF_INET, SOCK_STREAM, 0);

		if(SOCKET_FD < 0) {
			std::cerr << "Failed to create socket\n";
			return false;
		}

		return true;
	}
	// Returns true, if closes an existing Socket, else false
	bool closeSocket() {
		if(SOCKET_FD < 0) {
			return false;
		}

		close(SOCKET_FD);
		SOCKET_FD= -1;

		return true;
	}

	// --------------- Data Transfer Functions ---------------
	// Returns true, if messag was sent successfully, else false
	bool sendData(const string& message) {
		ssize_t bytes_sent= send(
			SOCKET_FD,
			message.data(),
			message.size(),
			0		
		);	

		return (bytes_sent >= 0);
	}
	// Returns the message received, if buffer is received successfully, else an empty string
	string receiveData() {
		string buffer(1024, '\0');

		ssize_t bytes_received= recv(
			SOCKET_FD,
			&buffer[0],
			buffer.size(),
			0			
		);

		if(bytes_received <= 0) {
			return "";			
		}

		buffer.resize(bytes_received);
		return buffer;
	}
};
