#include "connection.h"
#ifdef VG_MAC
#include "connection_mac.h"
#endif
#ifdef VG_WIN
#include "connection_win.h"
#endif

#include <iostream>

#include "shared/src/net/game_protocol.h"
#include "shared/src/net/net_packet.h"

#ifdef __clang__
#pragma clang diagnostic ignored "-Wswitch"
#endif

using namespace ME;

void ME::PlatformConnection::Init(const char* serverIP, uint16_t serverPort) {}

void ME::PlatformConnection::Update(double deltaTime) {}

void ME::PlatformConnection::End() {}

void ME::PlatformConnection::SendPacket(Packet* packet) {}

void ME::Connection::Init(const char* serverIP, uint16_t serverPort) {
#ifdef VG_MAC
    platformConnection = new ME::ConnectionMac();
#endif
#ifdef VG_WIN
    platformConnection = new ME::ConnectionWin();
#endif

    platformConnection->connection = this;
    platformConnection->Init(serverIP, serverPort);
}

void ME::Connection::Update(double deltaTime) {
    platformConnection->Update(deltaTime);
}

void ME::Connection::End() {
    platformConnection->End();
}

void ME::Connection::ProcessPacket(Packet& packet, uint32_t fromAddr, uint16_t fromPort) {
    uint8_t versionInt = packet.ReadByte();
    uint8_t verbInt = packet.ReadByte();
    uint8_t clientID = packet.ReadByte();
    ME::Net::GameProtocol::Verb verb = static_cast<ME::Net::GameProtocol::Verb>(verbInt);

    // std::cout << "Packet: Process: Verb: " << ME::Net::GameProtocol::GetVerbName(verbInt) << '\n';

    switch (verb) {
        case ME::Net::GameProtocol::Verb::CONNECTED:
            connectedServer.clientID = clientID;
            onConnected.Execute();
            break;
        case ME::Net::GameProtocol::Verb::PONG:
            onPong.Execute();
            break;
        case ME::Net::GameProtocol::Verb::CHAT_RECV:
            RecvChat(packet, clientID);
            break;
        case ME::Net::GameProtocol::Verb::DATA_RECV:
            RecvGameData(packet, clientID);
            break;
        case ME::Net::GameProtocol::Verb::SCORE_RECV:
            RecvScore(packet, clientID);
            break;
        case ME::Net::GameProtocol::Verb::HIGHSCORE_RECV:
            RecvHighScore(packet);
            break;
    }
}

void ME::Connection::SendConnectRequest() {
    PacketSmall packet;
    packet.WriteByte(ME::Net::GameProtocol::VERSION);
    packet.WriteByte(static_cast<uint8_t>(ME::Net::GameProtocol::Verb::CONNECT));
    SendPacket(&packet);
}

void ME::Connection::SendPing() {
    if (GetClientID() == 0xFF) return;  // Not a valid clientID. Not connected.

    PacketSmall packet;
    packet.WriteByte(ME::Net::GameProtocol::VERSION);
    packet.WriteByte(static_cast<uint8_t>(ME::Net::GameProtocol::Verb::PING));
    packet.WriteByte(connectedServer.clientID);
    SendPacket(&packet);
}

void ME::Connection::SendChat(const char* message) {
    if (GetClientID() == 0xFF) return;  // Not a valid clientID. Not connected.

    PacketSmall packet;
    packet.WriteByte(ME::Net::GameProtocol::VERSION);
    packet.WriteByte(static_cast<uint8_t>(ME::Net::GameProtocol::Verb::CHAT_SEND));
    packet.WriteByte(connectedServer.clientID);
    packet.WriteString(message);
    SendPacket(&packet);
}

void ME::Connection::RecvChat(Packet& packet, uint8_t clientID) {
    char messageBuffer[64];
    packet.ReadString(messageBuffer);
    std::cout << "Received Chat!\tFrom: " << ('A' + clientID) << ":\t" << messageBuffer << '\n';
}

void ME::Connection::SendGameData(const ME::FP_24_8& value1, const ME::FP_24_8& value2, const ME::FP_24_8& value3) {
    if (GetClientID() == 0xFF) return;  // Not a valid clientID. Not connected.

    PacketSmall packet;
    packet.WriteByte(ME::Net::GameProtocol::VERSION);
    packet.WriteByte(static_cast<uint8_t>(ME::Net::GameProtocol::Verb::DATA_SEND));
    packet.WriteByte(connectedServer.clientID);
    packet.WriteFP(value1);
    packet.WriteFP(value2);
    packet.WriteFP(value3);
    SendPacket(&packet);
}

void ME::Connection::RecvGameData(Packet& packet, uint8_t clientID) {
    ME::FP_24_8 value1 = packet.ReadFP();
    ME::FP_24_8 value2 = packet.ReadFP();
    ME::FP_24_8 value3 = packet.ReadFP();

    std::cout << "Received Data: " << value1.ToFloat() << ", " << value2.ToFloat() << ", " << value3.ToFloat() << '\n';
}

void ME::Connection::SendScore(uint32_t score) {
    if (GetClientID() == 0xFF) return;  // Not a valid clientID. Not connected.

    PacketSmall packet;
    packet.WriteByte(ME::Net::GameProtocol::VERSION);
    packet.WriteByte(static_cast<uint8_t>(ME::Net::GameProtocol::Verb::SCORE_SEND));
    packet.WriteByte(connectedServer.clientID);
    packet.WriteUInt32(score);
    SendPacket(&packet);
}

void ME::Connection::RecvScore(Packet& packet, uint8_t clientID) {
    lastScoreSenderID = clientID;
    lastScore = packet.ReadUInt32();
    onScoreReceived.Execute();
}

void ME::Connection::RecvHighScore(Packet& packet) {
    highScore = packet.ReadUInt32();
    onHighScoreReceived.Execute();
}

void ME::Connection::SendPacket(Packet* packet) {
    platformConnection->SendPacket(packet);
}

uint8_t ME::Connection::GetClientID() {
    return connectedServer.clientID;
}

bool ME::Connection::IsConnected() const {
    return connectedServer.clientID != 0xFF;
}

uint8_t ME::Connection::GetLastScoreSenderID() const {
    return lastScoreSenderID;
}

uint32_t ME::Connection::GetLastScore() const {
    return lastScore;
}

uint32_t ME::Connection::GetHighScore() const {
    return highScore;
}
