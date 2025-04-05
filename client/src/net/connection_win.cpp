#ifdef VG_WIN
#include "connection_win.h"

#include <winsock2.h>
#include <ws2tcpip.h>

#include <iostream>

#include "src/net/net_protocol.h"
#include "src/net/net_utils.h"

static const char* SERVER_IP = "127.0.0.1";
static const unsigned short PORT = 9310;

void ME::ConnectionWin::Init() {
    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed: " << WSAGetLastError() << "\n";
        return;
    }

    // Creating a UDP socket to connect to the erver.
    std::cout << "Client Connection Starting\n";
    clientSocketFD = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (clientSocketFD == -1) {
        std::cerr << "Socket creation failed\n";
        WSACleanup();
        return;
    }

    // Allow port reuse.
    const char opt = 1;
    if (setsockopt(clientSocketFD, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        std::cerr << "Allow port reuse failed\n";
        WSACleanup();
        return;
    }

    // Set socket to non-blocking mode
    u_long mode = 1;  // 1 for non-blocking
    if (ioctlsocket(clientSocketFD, FIONBIO, &mode) == SOCKET_ERROR) {
        std::cout << "Failed to set non-blocking: " << WSAGetLastError() << "\n";
        End();
        return;
    }
}

void ME::ConnectionWin::Update(double deltaTime) {
    while (true) {
        PacketMedium packet;

        sockaddr_in from;
        socklen_t fromLength = sizeof(from);

        int bytes =
            recvfrom(clientSocketFD, (char*)packet.GetData(), packet.GetSize(), 0, (sockaddr*)&from, &fromLength);
        if (bytes <= 0) break;

        uint32_t from_address = ntohl(from.sin_addr.s_addr);
        uint16_t from_port = ntohs(from.sin_port);

        connection->ProcessPacket(packet, from_address, from_port);
    }
}

void ME::ConnectionWin::End() {
    closesocket(clientSocketFD);
    WSACleanup();
}

void ME::ConnectionWin::SendPacket(Packet* packet) {
    // Define server address structure
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    int sent_bytes = sendto(clientSocketFD, (char*)packet->GetData(), packet->GetSize(), 0, (sockaddr*)&server_addr,
                            sizeof(sockaddr_in));

    if (sent_bytes != packet->GetSize()) {
        std::cout << "Failed to send packet.";
        return;
    }
}

#endif  // VG_WIN