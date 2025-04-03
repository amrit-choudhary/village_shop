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
void ME::SocketServer::Update(double deltaTime) {
    platformSocketServer->Update(deltaTime);
}

void ME::SocketServer::End() {
    platformSocketServer->End();
}

void ME::SocketServer::ProcessPacket(Packet& packet, uint32_t fromAddr, uint16_t fromPort) {
    uint8_t versionInt = packet.ReadByte();
    uint8_t verbInt = packet.ReadByte();
    uint8_t clientID = packet.ReadByte();
    ME::Net::Verb verb = static_cast<ME::Net::Verb>(verbInt);

    std::cout << "Packet: Verb: " << ME::Net::GetVerbName(verb) << ", " << ('A' + clientID) << '\n';

    switch (verb) {
        case ME::Net::Verb::CONNECT:
            ME::Net::ConnectedClient newClient;
            newClient.clientID = connectedClients.size();
            newClient.address = fromAddr;
            newClient.port = fromPort;

            connectedClients.push_back(newClient);
            SendConnected(connectedClients.size() - 1);
            break;
        case ME::Net::Verb::PING:
            SendPong(clientID);
            break;
        case ME::Net::Verb::CHAT_SEND:
            HandleChat(packet, clientID);
            break;
        case ME::Net::Verb::DATA_SEND:
            HandleData(packet, clientID);
            break;
    }
}

void ME::SocketServer::SendConnected(uint8_t clientID) {
    PacketSmall packet;
    packet.WriteByte(static_cast<uint8_t>(ME::Net::Version::VER_0));
    packet.WriteByte(static_cast<uint8_t>(ME::Net::Verb::CONNECTED));
    packet.WriteByte(static_cast<uint8_t>(clientID));
    SendPacket(&packet, clientID);
}

void ME::SocketServer::SendPong(uint8_t clientID) {
    PacketSmall packet;
    packet.WriteByte(static_cast<uint8_t>(ME::Net::Version::VER_0));
    packet.WriteByte(static_cast<uint8_t>(ME::Net::Verb::PONG));
    SendPacket(&packet, clientID);
}

void ME::SocketServer::HandleChat(Packet& packet, uint8_t clientID) {
    char messageBuffer[64];
    packet.ReadString(messageBuffer);
    std::vector<ME::Net::ConnectedClient> clients = GetAllClients();
    for (int i = 0; i < clients.size(); ++i) {
        if (clientID != clients[i].clientID) {
            PacketSmall packet;
            packet.WriteByte(static_cast<uint8_t>(ME::Net::Version::VER_0));
            packet.WriteByte(static_cast<uint8_t>(ME::Net::Verb::CHAT_RECV));
            packet.WriteByte(static_cast<uint8_t>(clientID));
            packet.WriteString(messageBuffer);
            SendPacket(&packet, clients[i].clientID);
        }
    }
}

void ME::SocketServer::HandleData(Packet& packet, uint8_t clientID) {
    ME::Math::FP_24_8 value1 = packet.ReadFP();
    ME::Math::FP_24_8 value2 = packet.ReadFP();
    ME::Math::FP_24_8 value3 = packet.ReadFP();

    std::vector<ME::Net::ConnectedClient> clients = GetAllClients();
    for (int i = 0; i < clients.size(); ++i) {
        if (clientID != clients[i].clientID) {
            PacketSmall packet;
            packet.WriteByte(static_cast<uint8_t>(ME::Net::Version::VER_0));
            packet.WriteByte(static_cast<uint8_t>(ME::Net::Verb::DATA_RECV));
            packet.WriteByte(static_cast<uint8_t>(clientID));
            packet.WriteFP(value1);
            packet.WriteFP(value2);
            packet.WriteFP(value3);
            SendPacket(&packet, clients[i].clientID);
        }
    }
}

void ME::SocketServer::SendPacket(Packet* packet, uint8_t clientID) {
    platformSocketServer->SendPacket(packet, clientID);
}

ME::Net::ConnectedClient ME::SocketServer::GetClient(uint8_t clientID) {
    return connectedClients[clientID];
}

std::vector<ME::Net::ConnectedClient> ME::SocketServer::GetAllClients() {
    return connectedClients;
}

void ME::PlatformSocketServer::Init() {}

void ME::PlatformSocketServer::Update(double deltaTime) {}

void ME::PlatformSocketServer::End() {}

void ME::PlatformSocketServer::SendPacket(Packet* packet, uint8_t clientID) {}
