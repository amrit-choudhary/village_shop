#include "connection_mac.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>

void ME::ConnectionMac::Init() {
    // Create socket (AF_INET for IPv4, SOCK_STREAM for TCP)
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        std::cerr << "Socket creation failed\n";
        return;
    }

    // Define server address structure
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;                      // IPv4
    server_addr.sin_port = htons(12345);                   // Port 12345
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  // Localhost

    // Connect to server
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Connection failed\n";
        return;
    }

    // Send message
    const char* message = "Hello from client!";
    if (write(sock, message, strlen(message)) < 0) {
        std::cerr << "Send failed\n";
        return;
    }

    // Receive response
    char buffer[1024] = {0};
    read(sock, buffer, 1024);
    std::cout << "Server response: " << buffer << "\n";

    return;
}

void ME::ConnectionMac::Update(double deltaTime) {}

void ME::ConnectionMac::End() {}

void ME::ConnectionMac::SendMessage(char* message) {
    if (write(sock, message, strlen(message)) < 0) {
        std::cerr << "Send failed\n";
        return;
    }
}
