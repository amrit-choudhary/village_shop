#include "game_client.h"

#include <cstring>

#include "logging/src/logging.h"
#include "shared/src/math/fp_24_8.h"
#include "shared/src/net/game_protocol.h"
#include "shared/src/serialization/byte_reader.h"
#include "shared/src/serialization/byte_writer.h"

namespace GameProtocol = ME::Net::GameProtocol;
using ME::Net::GameProtocol::Verb;

bool ME::GameClient::Init(const char* serverIP, uint16_t serverPort) {
    if (!Net::Address::Parse(serverIP, serverPort, connectedServer.address)) {
        LogError("Invalid game server address: ", serverIP);
        return false;
    }

    // Port 0: the OS picks a free port; the server replies to whatever port our datagrams come from.
    if (!socket.Open(0)) {
        return false;
    }
    LogInfo("Game server: ", serverIP, ":", serverPort);
    return true;
}

void ME::GameClient::Update(double deltaTime) {
    if (!socket.IsOpen()) {
        return;
    }

    uint8_t buffer[GameProtocol::MAX_DATAGRAM_SIZE];

    for (int i = 0; i < MAX_DATAGRAMS_PER_UPDATE; ++i) {
        int received = 0;
        Net::Address from;
        const Net::UdpResult result = socket.RecvFrom(buffer, GameProtocol::MAX_DATAGRAM_SIZE, received, from);
        if (result == Net::UdpResult::WouldBlock) {
            return;
        }
        if (result == Net::UdpResult::Error) {
            // Stop for this frame instead of retrying; a socket that keeps failing would spin the loop.
            LogWarning("Receiving datagram failed");
            return;
        }
        ProcessDatagram(buffer, received);
    }
}

void ME::GameClient::End() {
    socket.Close();
}

void ME::GameClient::ProcessDatagram(const uint8_t* data, int size) {
    ByteReader reader(data, static_cast<size_t>(size));
    GameProtocol::Header header;
    if (!GameProtocol::ReadHeader(reader, header)) {
        LogWarning("Dropped datagram shorter than a header (", size, " bytes)");
        return;
    }

    switch (static_cast<Verb>(header.verb)) {
        case Verb::CONNECTED:
            connectedServer.clientID = header.clientID;
            onConnected.Execute();
            break;
        case Verb::PONG:
            onPong.Execute();
            break;
        case Verb::CHAT_RECV:
            RecvChat(reader, header.clientID);
            break;
        case Verb::DATA_RECV:
            RecvGameData(reader);
            break;
        case Verb::SCORE_RECV:
            RecvScore(reader, header.clientID);
            break;
        case Verb::HIGHSCORE_RECV:
            RecvHighScore(reader);
            break;
        case Verb::QUIZ_QUESTION:
            RecvQuizQuestion(reader);
            break;
        default:
            break;
    }
}

void ME::GameClient::SendConnectRequest() {
    uint8_t buffer[GameProtocol::MAX_DATAGRAM_SIZE];
    ByteWriter writer(buffer, sizeof(buffer));
    GameProtocol::WriteHeader(writer, Verb::CONNECT, connectedServer.clientID);
    SendDatagram(buffer, writer.GetSize());
}

void ME::GameClient::SendPing() {
    if (!IsConnected()) {
        return;
    }

    uint8_t buffer[GameProtocol::MAX_DATAGRAM_SIZE];
    ByteWriter writer(buffer, sizeof(buffer));
    GameProtocol::WriteHeader(writer, Verb::PING, connectedServer.clientID);
    SendDatagram(buffer, writer.GetSize());
}

void ME::GameClient::SendChat(const char* message) {
    if (!IsConnected()) {
        return;
    }
    if (std::strlen(message) >= GameProtocol::CHAT_CAPACITY) {
        LogWarning("Chat not sent: longer than ", GameProtocol::CHAT_CAPACITY - 1, " characters");
        return;
    }

    uint8_t buffer[GameProtocol::MAX_DATAGRAM_SIZE];
    ByteWriter writer(buffer, sizeof(buffer));
    GameProtocol::WriteHeader(writer, Verb::CHAT_SEND, connectedServer.clientID);
    GameProtocol::WriteString(writer, message);
    SendDatagram(buffer, writer.GetSize());
}

void ME::GameClient::SendGameData(const FP_24_8& value1, const FP_24_8& value2, const FP_24_8& value3) {
    if (!IsConnected()) {
        return;
    }

    uint8_t buffer[GameProtocol::MAX_DATAGRAM_SIZE];
    ByteWriter writer(buffer, sizeof(buffer));
    GameProtocol::WriteHeader(writer, Verb::DATA_SEND, connectedServer.clientID);
    GameProtocol::WriteFP(writer, value1);
    GameProtocol::WriteFP(writer, value2);
    GameProtocol::WriteFP(writer, value3);
    SendDatagram(buffer, writer.GetSize());
}

void ME::GameClient::SendScore(uint32_t score) {
    if (!IsConnected()) {
        return;
    }

    uint8_t buffer[GameProtocol::MAX_DATAGRAM_SIZE];
    ByteWriter writer(buffer, sizeof(buffer));
    GameProtocol::WriteHeader(writer, Verb::SCORE_SEND, connectedServer.clientID);
    writer.WriteU32(score);
    SendDatagram(buffer, writer.GetSize());
}

void ME::GameClient::RecvChat(ByteReader& reader, uint8_t clientID) {
    char message[GameProtocol::CHAT_CAPACITY];
    if (!GameProtocol::ReadString(reader, message, sizeof(message))) {
        LogWarning("Dropped invalid chat");
        return;
    }
    LogInfo("Chat from client ", static_cast<unsigned>(clientID), ": ", message);
}

void ME::GameClient::RecvGameData(ByteReader& reader) {
    FP_24_8 value1;
    FP_24_8 value2;
    FP_24_8 value3;
    if (!GameProtocol::ReadFP(reader, value1) || !GameProtocol::ReadFP(reader, value2) ||
        !GameProtocol::ReadFP(reader, value3)) {
        LogWarning("Dropped short game data");
        return;
    }
    LogInfo("Received data: ", value1.ToFloat(), ", ", value2.ToFloat(), ", ", value3.ToFloat());
}

void ME::GameClient::RecvScore(ByteReader& reader, uint8_t clientID) {
    uint32_t score = 0;
    if (!reader.ReadU32(score)) {
        LogWarning("Dropped short score");
        return;
    }
    lastScoreSenderID = clientID;
    lastScore = score;
    onScoreReceived.Execute();
}

void ME::GameClient::RecvHighScore(ByteReader& reader) {
    uint32_t score = 0;
    if (!reader.ReadU32(score)) {
        LogWarning("Dropped short high score");
        return;
    }
    highScore = score;
    onHighScoreReceived.Execute();
}

void ME::GameClient::RecvQuizQuestion(ByteReader& reader) {
    GameProtocol::QuizQuestion question;
    if (!GameProtocol::ReadQuizQuestion(reader, question)) {
        LogWarning("Dropped invalid quiz question");
        return;
    }
    lastQuizQuestion = question;
    onQuizQuestion.Execute();
}

void ME::GameClient::SendDatagram(const uint8_t* data, size_t size) {
    if (!socket.IsOpen()) {
        return;
    }
    if (socket.SendTo(data, static_cast<int>(size), connectedServer.address) != Net::UdpResult::Ok) {
        LogWarning("Sending to game server failed");
    }
}

uint8_t ME::GameClient::GetClientID() const {
    return connectedServer.clientID;
}

bool ME::GameClient::IsConnected() const {
    return connectedServer.clientID != 0xFF;
}

uint8_t ME::GameClient::GetLastScoreSenderID() const {
    return lastScoreSenderID;
}

uint32_t ME::GameClient::GetLastScore() const {
    return lastScore;
}

uint32_t ME::GameClient::GetHighScore() const {
    return highScore;
}

const ME::Net::GameProtocol::QuizQuestion& ME::GameClient::GetLastQuizQuestion() const {
    return lastQuizQuestion;
}
