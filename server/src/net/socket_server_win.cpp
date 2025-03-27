#ifdef VG_WIN

#include "socket_server_win.h"

#include <winsock2.h>
#include <ws2tcpip.h>

#include <iostream>

void ME::SocketServerWin::Init() {
    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed: " << WSAGetLastError() << "\n";
        return;
    }

    // Create socket
    SOCKET server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_fd == INVALID_SOCKET) {
        std::cerr << "Socket creation failed: " << WSAGetLastError() << "\n";
        WSACleanup();
        return;
    }

    // Allow port reuse
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt)) == SOCKET_ERROR) {
        std::cerr << "Setsockopt failed: " << WSAGetLastError() << "\n";
        closesocket(server_fd);
        WSACleanup();
        return;
    }

    // Define server address structure
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(12345);

    // Bind and listen
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed: " << WSAGetLastError() << "\n";
        closesocket(server_fd);
        WSACleanup();
        return;
    }
    if (listen(server_fd, 1) == SOCKET_ERROR) {
        std::cerr << "Listen failed: " << WSAGetLastError() << "\n";
        closesocket(server_fd);
        WSACleanup();
        return;
    }
    std::cout << "Server listening on port 12345...\n";

    while (true) {
        // Accept incoming connection
        sockaddr_in client_addr;
        int client_len = sizeof(client_addr);
        SOCKET client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
        if (client_fd == INVALID_SOCKET) {
            std::cerr << "Accept failed: " << WSAGetLastError() << "\n";
            continue;
        }
        std::cout << "Client connected\n";

        // Handle client communication in a loop
        while (true) {
            char buffer[1024] = {0};
            int bytes_received = recv(client_fd, buffer, 1024, 0);
            if (bytes_received == SOCKET_ERROR || bytes_received == 0) {
                std::cout << "Client disconnected: " << WSAGetLastError() << "\n";
                break;
            }
            std::cout << "Received: " << buffer << "\n";

            // Send response
            const char* response = "Message received!";
            if (send(client_fd, response, strlen(response), 0) == SOCKET_ERROR) {
                std::cerr << "Send failed: " << WSAGetLastError() << "\n";
                break;
            }
        }

        // Close client connection
        closesocket(client_fd);
    }

    // Cleanup
    closesocket(server_fd);
    WSACleanup();
    return;
}

void ME::SocketServerWin::Update(double deltaTime) {}

void ME::SocketServerWin::End() {}

#endif  // VG_WIN