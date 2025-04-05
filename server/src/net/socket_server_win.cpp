#ifdef VG_WIN

#include "socket_server_win.h"

#include <winsock2.h>
#include <ws2tcpip.h>

#include <chrono>
#include <iostream>
#include <thread>

#include "src/net/net_packet.h"

constexpr unsigned short PORT = 9310;

void ME::SocketServerWin::Init() {
    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed: " << WSAGetLastError() << "\n";
        return;
    }

    serverSockerFd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (serverSockerFd == INVALID_SOCKET) {
        std::cerr << "Socket creation failed: " << WSAGetLastError() << "\n";
        WSACleanup();
        return;
    }

    // Allow port reuse.
    const char opt = 1;
    if (setsockopt(serverSockerFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        std::cerr << "Allow port reuse failed\n";
        WSACleanup();
        return;
    }

    // Set socket to non-blocking mode
    u_long mode = 1;  // 1 for non-blocking
    if (ioctlsocket(serverSockerFd, FIONBIO, &mode) == SOCKET_ERROR) {
        std::cout << "Failed to set non-blocking: " << WSAGetLastError() << "\n";
        End();
        return;
    }

    // Define server address structure
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind server to socket.
    if (bind(serverSockerFd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed: " << WSAGetLastError() << "\n";
        closesocket(serverSockerFd);
        WSACleanup();
        return;
    }

    std::cout << "Server listening on port: " << ntohs(server_addr.sin_port) << '\n';
}

void ME::SocketServerWin::Update(double deltaTime) {
    while (true) {
        PacketMedium packet;

        sockaddr_in from;
        socklen_t fromLength = sizeof(from);

        int bytes =
            recvfrom(serverSockerFd, (char*)(packet.GetData()), packet.GetSize(), 0, (sockaddr*)&from, &fromLength);
        if (bytes <= 0) break;

        uint32_t from_address = ntohl(from.sin_addr.s_addr);
        uint16_t from_port = ntohs(from.sin_port);

        socketServer->ProcessPacket(packet, from_address, from_port);
    }
}

void ME::SocketServerWin::SendPacket(Packet* packet, uint8_t clientID) {
    // Define client address structure
    ME::Net::ConnectedClient client = socketServer->GetClient(clientID);
    sockaddr_in client_addr;
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(client.port);
    client_addr.sin_addr.s_addr = htonl(client.address);

    int sent_bytes = sendto(serverSockerFd, (char*)packet->GetData(), packet->GetSize(), 0, (sockaddr*)&client_addr,
                            sizeof(sockaddr_in));

    if (sent_bytes != packet->GetSize()) {
        std::cout << "Failed to send packet.";
        return;
    }
}

void ME::SocketServerWin::End() {
    closesocket(serverSockerFd);
    WSACleanup();
}

#endif  // VG_WIN