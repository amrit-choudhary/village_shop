#ifdef VG_MAC

#include "socket_server_mac.h"

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>

void ME::SocketServerMac::Init() {
    // Create socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        std::cerr << "Socket creation failed\n";
        return;
    }

    // Allow port reuse
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        std::cerr << "Setsockopt failed\n";
        return;
    }

    // Define server address structure
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(12345);

    // Bind and listen
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Bind failed\n";
        return;
    }
    if (listen(server_fd, 1) < 0) {
        std::cerr << "Listen failed\n";
        return;
    }
    std::cout << "Server listening on port 12345...\n";

    while (true) {
        // Accept incoming connection
        sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
        if (client_fd < 0) {
            std::cerr << "Accept failed\n";
            continue;
        }
        std::cout << "Client connected\n";

        // Handle client communication in a loop
        while (true) {
            char buffer[1024] = {0};
            int bytes_received = read(client_fd, buffer, 1024);
            if (bytes_received <= 0) {
                std::cout << "Client disconnected\n";
                break;
            }
            std::cout << "Received: " << buffer << "\n";

            // Send response
            const char* response = "Message received!";
            if (write(client_fd, response, strlen(response)) < 0) {
                std::cerr << "Send failed\n";
                break;
            }
        }

        // Close client connection
        close(client_fd);
    }

    close(server_fd);
    return;
}

void ME::SocketServerMac::Update(double deltaTime) {}

void ME::SocketServerMac::End() {}

#endif  // VG_MAC
