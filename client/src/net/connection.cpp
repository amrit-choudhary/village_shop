#include "connection.h"
#ifdef VG_MAC
#include "connection_mac.h"
#endif

#include "src/net/net_packet.h"
#include "src/net/net_protocol.h"

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
    platformConnection->Init();
}

void ME::Connection::Update(double deltaTime) { platformConnection->Update(deltaTime); }

void ME::Connection::End() { platformConnection->End(); }

void ME::Connection::SendMessage(char* message) { platformConnection->SendMessage(message); }

void ME::Connection::SendConnectRequest() {}

void ME::Connection::SendPing() {
    PacketSmall packet;
    packet.WriteByte(static_cast<uint8_t>(ME::Net::Version::VER_3));
    packet.WriteByte(static_cast<uint8_t>(ME::Net::Verb::GET));
    SendPacket(&packet);
}

void ME::Connection::SendPacket(Packet* packet) { platformConnection->SendPacket(packet); }
