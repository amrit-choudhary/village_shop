#include "connection.h"
#ifdef VG_MAC
#include "connection_mac.h"
#endif

#include <iostream>

#include "src/net/net_packet.h"
#include "src/net/net_protocol.h"
#include "src/net/net_utils.h"

#pragma clang diagnostic ignored "-Wswitch"

using namespace ME;

void ME::PlatformConnection::Init() {}

void ME::PlatformConnection::Update(double deltaTime) {}

void ME::PlatformConnection::End() {}

void ME::PlatformConnection::SendMessage(char* message) {}

void ME::PlatformConnection::SendPacket(Packet* packet) {}

void ME::Connection::Init() {
#ifdef VG_MAC
    platformConnection = new ME::ConnectionMac();
#endif

    platformConnection->connection = this;
    platformConnection->Init();
}

void ME::Connection::Update(double deltaTime) { platformConnection->Update(deltaTime); }

void ME::Connection::End() { platformConnection->End(); }

void ME::Connection::SendMessage(char* message) { platformConnection->SendMessage(message); }

void ME::Connection::SendConnectRequest() {
    PacketSmall packet;
    packet.WriteByte(static_cast<uint8_t>(ME::Net::Version::VER_0));
    packet.WriteByte(static_cast<uint8_t>(ME::Net::Verb::CONNECT));
    SendPacket(&packet);
}

void ME::Connection::SendPing() {
    if (GetClientID() == 0xFF) return;  // Not a valid clientID. Not connected.

    PacketSmall packet;
    packet.WriteByte(static_cast<uint8_t>(ME::Net::Version::VER_0));
    packet.WriteByte(static_cast<uint8_t>(ME::Net::Verb::PING));
    packet.WriteByte(connectedServer.clientID);
    SendPacket(&packet);
}

void ME::Connection::SendChat(const char* message) {
    if (GetClientID() == 0xFF) return;  // Not a valid clientID. Not connected.

    PacketSmall packet;
    packet.WriteByte(static_cast<uint8_t>(ME::Net::Version::VER_0));
    packet.WriteByte(static_cast<uint8_t>(ME::Net::Verb::CHAT_SEND));
    packet.WriteByte(connectedServer.clientID);
    packet.WriteString(message);
    SendPacket(&packet);
}

void ME::Connection::RecvChat(Packet& packet) { std::cout << "Received Chat!" << '\n'; }

void ME::Connection::SendPacket(Packet* packet) { platformConnection->SendPacket(packet); }

void ME::Connection::ProcessPacket(Packet& packet, uint32_t fromAddr, uint16_t fromPort) {
    uint8_t versionInt = packet.ReadByte();
    uint8_t verbInt = packet.ReadByte();
    uint8_t clientID = packet.ReadByte();
    ME::Net::Verb verb = static_cast<ME::Net::Verb>(verbInt);

    std::cout << "Packet: Process: Verb: " << ME::Net::GetVerbName(verb) << '\n';

    switch (verb) {
        case ME::Net::Verb::CONNECTED:
            connectedServer.clientID = clientID;
            break;
        case ME::Net::Verb::PONG:
            break;
        case ME::Net::Verb::CHAT_RECV:
            RecvChat(packet);
            break;
    }
}

uint8_t ME::Connection::GetClientID() { return connectedServer.clientID; }
