#include "game_server.h"

#include "logging/src/logging.h"
#include "shared/src/math/fp_24_8.h"
#include "shared/src/misc/utils.h"
#include "shared/src/net/game_protocol.h"
#include "shared/src/serialization/byte_reader.h"
#include "shared/src/serialization/byte_writer.h"

namespace GameProtocol = ME::Net::GameProtocol;
using ME::Net::GameProtocol::Verb;

bool ME::GameServer::Init(uint16_t port) {
    scoreDB.Open((ME::Utils::GetExecutableDirPath() + "/village_shop.db").c_str());

    if (!socket.Open(port)) {
        return false;
    }
    LogSuccess("Server listening on UDP port ", port);
    return true;
}

void ME::GameServer::Update(double deltaTime) {
    uint8_t buffer[GameProtocol::MAX_DATAGRAM_SIZE];

    for (int i = 0; i < MAX_DATAGRAMS_PER_UPDATE; ++i) {
        int received = 0;
        Net::Address from;
        const Net::UdpResult result = socket.RecvFrom(buffer, GameProtocol::MAX_DATAGRAM_SIZE, received, from);
        if (result == Net::UdpResult::WouldBlock) {
            return;
        }
        if (result == Net::UdpResult::Error) {
            // Stop for this tick instead of retrying; a socket that keeps failing would spin the loop.
            LogWarning("Receiving datagram failed");
            return;
        }
        ProcessDatagram(buffer, received, from);
    }
}

void ME::GameServer::End() {
    socket.Close();
    scoreDB.Close();
}

void ME::GameServer::SetTimerManagerRef(Time::TimerManager* ptrTimerManager) {
    timerManager = ptrTimerManager;
}

void ME::GameServer::ProcessDatagram(const uint8_t* data, int size, const Net::Address& from) {
    ByteReader reader(data, static_cast<size_t>(size));
    GameProtocol::Header header;
    if (!GameProtocol::ReadHeader(reader, header)) {
        LogWarning("Dropped datagram shorter than a header (", size, " bytes)");
        return;
    }

    LogInfo("Datagram: ", GameProtocol::GetVerbName(header.verb), ", client ",
            static_cast<unsigned>(header.clientID));

    switch (static_cast<Verb>(header.verb)) {
        case Verb::CONNECT:
            HandleConnect(from);
            break;
        case Verb::PING:
            SendPong(header.clientID);
            break;
        case Verb::CHAT_SEND:
            HandleChat(reader, header.clientID);
            break;
        case Verb::DATA_SEND:
            HandleData(reader, header.clientID);
            break;
        case Verb::SCORE_SEND:
            HandleScore(reader, header.clientID);
            break;
        default:
            break;
    }
}

void ME::GameServer::HandleConnect(const Net::Address& from) {
    ConnectedClient newClient;
    newClient.clientID = static_cast<uint8_t>(connectedClients.size());
    newClient.address = from;
    connectedClients.push_back(newClient);

    SendConnected(newClient.clientID);
    SendHighScore(newClient.clientID);
}

void ME::GameServer::HandleChat(ByteReader& reader, uint8_t clientID) {
    char message[GameProtocol::CHAT_CAPACITY];
    if (!GameProtocol::ReadString(reader, message, sizeof(message))) {
        LogWarning("Dropped invalid chat from client ", static_cast<unsigned>(clientID));
        return;
    }

    uint8_t buffer[GameProtocol::MAX_DATAGRAM_SIZE];
    ByteWriter writer(buffer, sizeof(buffer));
    GameProtocol::WriteHeader(writer, Verb::CHAT_RECV, clientID);
    GameProtocol::WriteString(writer, message);
    SendToOthers(buffer, writer.GetSize(), clientID);
}

void ME::GameServer::HandleData(ByteReader& reader, uint8_t clientID) {
    FP_24_8 value1;
    FP_24_8 value2;
    FP_24_8 value3;
    if (!GameProtocol::ReadFP(reader, value1) || !GameProtocol::ReadFP(reader, value2) ||
        !GameProtocol::ReadFP(reader, value3)) {
        LogWarning("Dropped short data from client ", static_cast<unsigned>(clientID));
        return;
    }

    uint8_t buffer[GameProtocol::MAX_DATAGRAM_SIZE];
    ByteWriter writer(buffer, sizeof(buffer));
    GameProtocol::WriteHeader(writer, Verb::DATA_RECV, clientID);
    GameProtocol::WriteFP(writer, value1);
    GameProtocol::WriteFP(writer, value2);
    GameProtocol::WriteFP(writer, value3);
    SendToOthers(buffer, writer.GetSize(), clientID);
}

void ME::GameServer::HandleScore(ByteReader& reader, uint8_t clientID) {
    uint32_t score = 0;
    if (!reader.ReadU32(score)) {
        LogWarning("Dropped short score from client ", static_cast<unsigned>(clientID));
        return;
    }

    uint8_t buffer[GameProtocol::MAX_DATAGRAM_SIZE];
    ByteWriter writer(buffer, sizeof(buffer));
    GameProtocol::WriteHeader(writer, Verb::SCORE_RECV, clientID);
    writer.WriteU32(score);
    SendToOthers(buffer, writer.GetSize(), clientID);

    // A new global high score goes to every client, including the sender.
    if (scoreDB.SubmitScore(score)) {
        for (const ConnectedClient& client : connectedClients) {
            SendHighScore(client.clientID);
        }
    }
}

void ME::GameServer::SendConnected(uint8_t clientID) {
    uint8_t buffer[GameProtocol::MAX_DATAGRAM_SIZE];
    ByteWriter writer(buffer, sizeof(buffer));
    GameProtocol::WriteHeader(writer, Verb::CONNECTED, clientID);
    SendDatagram(buffer, writer.GetSize(), clientID);
}

void ME::GameServer::SendPong(uint8_t clientID) {
    uint8_t buffer[GameProtocol::MAX_DATAGRAM_SIZE];
    ByteWriter writer(buffer, sizeof(buffer));
    GameProtocol::WriteHeader(writer, Verb::PONG, clientID);
    SendDatagram(buffer, writer.GetSize(), clientID);
}

void ME::GameServer::SendHighScore(uint8_t clientID) {
    uint8_t buffer[GameProtocol::MAX_DATAGRAM_SIZE];
    ByteWriter writer(buffer, sizeof(buffer));
    GameProtocol::WriteHeader(writer, Verb::HIGHSCORE_RECV, clientID);
    writer.WriteU32(scoreDB.GetHighScore());
    SendDatagram(buffer, writer.GetSize(), clientID);
}

void ME::GameServer::SendDatagram(const uint8_t* data, size_t size, uint8_t clientID) {
    const Net::Address& to = connectedClients[clientID].address;
    if (socket.SendTo(data, static_cast<int>(size), to) != Net::UdpResult::Ok) {
        LogWarning("Sending to client ", static_cast<unsigned>(clientID), " failed");
    }
}

void ME::GameServer::SendToOthers(const uint8_t* data, size_t size, uint8_t exceptClientID) {
    for (const ConnectedClient& client : connectedClients) {
        if (client.clientID != exceptClientID) {
            SendDatagram(data, size, client.clientID);
        }
    }
}
