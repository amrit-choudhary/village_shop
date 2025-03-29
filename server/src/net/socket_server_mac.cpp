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
    serverSockerFd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (serverSockerFd == -1) {
        std::cerr << "Socket creation failed\n";
        return;
    }

    // Allow port reuse.
    int opt = 1;
    if (setsockopt(serverSockerFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        std::cerr << "Allow port reuse failed\n";
        return;
    }

    // Set socket to non-blocking mode.
    int nonBlocking = 1;
    if (fcntl(serverSockerFd, F_SETFL, O_NONBLOCK, nonBlocking) == -1) {
        std::cout << "Failed to set non blocking.";
        return;
    }

    // Server address.
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind server to socket.
    if (bind(serverSockerFd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
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

        int bytes = recvfrom(serverSockerFd, packet.GetData(), packet.GetSize(), 0, (sockaddr*)&from, &fromLength);

        if (bytes <= 0) break;

        unsigned int from_address = ntohl(from.sin_addr.s_addr);

        unsigned int from_port = ntohs(from.sin_port);

        uint8_t version = packet.ReadByte();
        uint8_t verb = packet.ReadByte();

        std::cout << from_address << ", " << from_port << ", " << ('A' + version) << ", " << ('A' + verb) << '\n';

        // process received packet
    }
}

void ME::SocketServerMac::End() { close(serverSockerFd); }

#endif  // VG_MAC
