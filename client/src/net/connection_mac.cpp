#ifdef VG_MAC

#include "connection_mac.h"

#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>

#include "src/net/net_protocol.h"
#include "src/net/net_utils.h"

static const char* SERVER_IP = "127.0.0.1";
static const unsigned short PORT = 9310;

void ME::ConnectionMac::Init() {
    // Creating a UDP socket to connect to the erver.
    std::cout << "Client Connection Starting\n";
    clientSockerFd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (clientSockerFd == -1) {
        std::cerr << "Socket creation failed\n";
        return;
    }

    // Allow port reuse.
    int opt = 1;
    if (setsockopt(clientSockerFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        std::cerr << "Allow port reuse failed\n";
        return;
    }

    // Set socket to non-blocking mode.
    int nonBlocking = 1;
    if (fcntl(clientSockerFd, F_SETFL, O_NONBLOCK, nonBlocking) == -1) {
        std::cout << "Failed to set non blocking.";
        return;
    }
}

void ME::ConnectionMac::Update(double deltaTime) {
    while (true) {
        PacketMedium packet;

        sockaddr_in from;
        socklen_t fromLength = sizeof(from);

        int bytes = recvfrom(clientSockerFd, packet.GetData(), packet.GetSize(), 0, (sockaddr*)&from, &fromLength);
        if (bytes <= 0) break;

        uint32_t from_address = ntohl(from.sin_addr.s_addr);
        uint16_t from_port = ntohs(from.sin_port);

        connection->ProcessPacket(packet, from_address, from_port);
    }
}

void ME::ConnectionMac::End() {
    close(clientSockerFd);
}

void ME::ConnectionMac::SendMessage(char* message) {
    // Define server address structure
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    uint8_t packet_data[256] = {0};
    *(packet_data) = (uint8_t)ME::Net::Version::VER_0;
    *(packet_data + 1) = (uint8_t)ME::Net::Verb::PING;
    uint8_t packet_size = 2;

    int sent_bytes = sendto(clientSockerFd, packet_data, packet_size, 0, (sockaddr*)&server_addr, sizeof(sockaddr_in));

    if (sent_bytes != packet_size) {
        std::cout << "Failed to send packet.";
        return;
    }
}

void ME::ConnectionMac::SendPacket(Packet* packet) {
    // Define server address structure
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    int sent_bytes =
        sendto(clientSockerFd, packet->GetData(), packet->GetSize(), 0, (sockaddr*)&server_addr, sizeof(sockaddr_in));

    if (sent_bytes != packet->GetSize()) {
        std::cout << "Failed to send packet.";
        return;
    }
}

#endif  // VG_MAC