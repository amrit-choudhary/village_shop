#include "socket_server.h"
#ifdef VG_MAC
#include "socket_server_mac.h"
#endif
#ifdef VG_WIN
#include "socket_server_win.h"
#endif

#include <iostream>

#include "src/net/net_packet.h"
#include "src/net/net_protocol.h"
#include "src/net/net_utils.h"

#pragma clang diagnostic ignored "-Wswitch"

void ME::SocketServer::Init() {
#ifdef VG_MAC
    platformSocketServer = new ME::SocketServerMac();
#endif
#ifdef VG_WIN
    platformSocketServer = new ME::SocketServerWin();
#endif

    platformSocketServer->socketServer = this;
    platformSocketServer->Init();
}
void ME::SocketServer::Update(double deltaTime) { platformSocketServer->Update(deltaTime); }

void ME::SocketServer::End() { platformSocketServer->End(); }

void ME::SocketServer::ProcessPacket(Packet& packet, uint32_t fromAddr, uint16_t fromPort) {
    uint8_t versionInt = packet.ReadByte();
    uint8_t verbInt = packet.ReadByte();
    ME::Net::Verb verb = static_cast<ME::Net::Verb>(verbInt);

    std::cout << "Packet: Verb: " << ME::Net::GetVerbName(verb) << '\n';

    switch (verb) {
        case ME::Net::Verb::CONNECT:
            ME::Net::ConnectedClient newClient;
            newClient.clientID = connectedClients.size();
            newClient.address = fromAddr;
            newClient.port = fromPort;

            connectedClients.push_back(newClient);
            break;
        case ME::Net::Verb::PING:
            SendPong();
            break;
    }
}

void ME::SocketServer::SendPong() {
    PacketSmall packet;
    packet.WriteByte(static_cast<uint8_t>(ME::Net::Version::VER_0));
    packet.WriteByte(static_cast<uint8_t>(ME::Net::Verb::PONG));
    SendPacket(&packet);
}

void ME::SocketServer::SendPacket(Packet* packet) { platformSocketServer->SendPacket(packet); }

ME::Net::ConnectedClient ME::SocketServer::GetClient() { return connectedClients[0]; }

void ME::PlatformSocketServer::Init() {}

void ME::PlatformSocketServer::Update(double deltaTime) {}

void ME::PlatformSocketServer::End() {}

void ME::PlatformSocketServer::SendPacket(Packet* packet) {}
