#ifdef VG_MAC

#include "socket_server_mac.h"

#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <chrono>
#include <iostream>
#include <thread>

#include "src/net/net_packet.h"

constexpr unsigned short PORT = 9310;

void ME::SocketServerMac::Init() {
    std::cout << "Server Starting\n";
    serverSocketFD = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (serverSocketFD == -1) {
        std::cerr << "Socket creation failed\n";
        return;
    }

    // Allow port reuse.
    int opt = 1;
    if (setsockopt(serverSocketFD, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        std::cerr << "Allow port reuse failed\n";
        return;
    }

    // Set socket to non-blocking mode.
    int nonBlocking = 1;
    if (fcntl(serverSocketFD, F_SETFL, O_NONBLOCK, nonBlocking) == -1) {
        std::cout << "Failed to set non blocking.";
        return;
    }

    // Server address.
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind server to socket.
    if (bind(serverSocketFD, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Bind failed\n";
        return;
    }

    std::cout << "Server listening on port: " << ntohs(server_addr.sin_port) << '\n';
}

void ME::SocketServerMac::Update(double deltaTime) {
    while (true) {
        PacketMedium packet;

        sockaddr_in from;
        socklen_t fromLength = sizeof(from);

        int bytes = recvfrom(serverSocketFD, packet.GetData(), packet.GetSize(), 0, (sockaddr*)&from, &fromLength);
        if (bytes <= 0) break;

        uint32_t from_address = ntohl(from.sin_addr.s_addr);
        uint16_t from_port = ntohs(from.sin_port);

        socketServer->ProcessPacket(packet, from_address, from_port);
    }
}

void ME::SocketServerMac::SendPacket(Packet* packet, uint8_t clientID) {
    // Define client address structure
    ME::Net::ConnectedClient client = socketServer->GetClient(clientID);
    sockaddr_in client_addr;
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(client.port);
    client_addr.sin_addr.s_addr = htonl(client.address);

    int sent_bytes =
        sendto(serverSocketFD, packet->GetData(), packet->GetSize(), 0, (sockaddr*)&client_addr, sizeof(sockaddr_in));

    if (sent_bytes != packet->GetSize()) {
        std::cout << "Failed to send packet.";
        return;
    }
}

void ME::SocketServerMac::End() {
    close(serverSocketFD);
}

#endif  // VG_MAC
