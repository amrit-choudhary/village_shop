#include "game_protocol.h"

#include <cstring>

#include "shared/src/math/fp_24_8.h"
#include "shared/src/serialization/byte_reader.h"
#include "shared/src/serialization/byte_writer.h"

bool ME::Net::GameProtocol::WriteHeader(ByteWriter& writer, Verb verb, uint8_t clientID) {
    return writer.WriteU8(VERSION) && writer.WriteU8(static_cast<uint8_t>(verb)) && writer.WriteU8(clientID);
}

bool ME::Net::GameProtocol::ReadHeader(ByteReader& reader, Header& out) {
    Header header;
    if (!reader.ReadU8(header.version) || !reader.ReadU8(header.verb) || !reader.ReadU8(header.clientID)) {
        return false;
    }
    out = header;
    return true;
}

bool ME::Net::GameProtocol::WriteFP(ByteWriter& writer, const FP_24_8& value) {
    return writer.WriteU32(static_cast<uint32_t>(value.GetRaw()));
}

bool ME::Net::GameProtocol::ReadFP(ByteReader& reader, FP_24_8& out) {
    uint32_t raw = 0;
    if (!reader.ReadU32(raw)) {
        return false;
    }
    out = FP_24_8(static_cast<int32_t>(raw), true);
    return true;
}

bool ME::Net::GameProtocol::WriteString(ByteWriter& writer, const char* text) {
    const size_t length = std::strlen(text);
    // Check room for length byte + text up front, so a failed write leaves nothing half-written.
    if (length > MAX_STRING_LENGTH || writer.GetRemaining() < 1 + length) {
        return false;
    }
    return writer.WriteU8(static_cast<uint8_t>(length)) &&
           writer.WriteBytes(reinterpret_cast<const uint8_t*>(text), length);
}

bool ME::Net::GameProtocol::ReadString(ByteReader& reader, char* out, size_t capacity) {
    uint8_t length = 0;
    if (!reader.ReadU8(length)) {
        return false;
    }
    // + 1: out also needs room for the terminating 0, which is added here and not sent.
    if (static_cast<size_t>(length) + 1 > capacity) {
        return false;
    }

    const uint8_t* text = nullptr;
    if (!reader.ReadBytes(length, text)) {
        return false;
    }
    std::memcpy(out, text, length);
    out[length] = '\0';
    return true;
}

bool ME::Net::GameProtocol::WriteQuizQuestion(ByteWriter& writer, const QuizQuestion& question) {
    return writer.WriteU32(question.id) && writer.WriteU8(question.lhs) &&
           writer.WriteU8(static_cast<uint8_t>(question.op)) && writer.WriteU8(question.rhs) &&
           writer.WriteU8(question.options[0]) && writer.WriteU8(question.options[1]);
}

bool ME::Net::GameProtocol::ReadQuizQuestion(ByteReader& reader, QuizQuestion& out) {
    QuizQuestion question;
    uint8_t op = 0;
    if (!reader.ReadU32(question.id) || !reader.ReadU8(question.lhs) || !reader.ReadU8(op) ||
        !reader.ReadU8(question.rhs) || !reader.ReadU8(question.options[0]) || !reader.ReadU8(question.options[1])) {
        return false;
    }
    if (op != '+' && op != '-') {
        return false;
    }
    question.op = static_cast<char>(op);
    out = question;
    return true;
}

const char* ME::Net::GameProtocol::GetVerbName(uint8_t verb) {
    switch (static_cast<Verb>(verb)) {
        case Verb::NONE:
            return "NONE";
        case Verb::ACK:
            return "ACK";
        case Verb::AUTH:
            return "AUTH";
        case Verb::PING:
            return "PING";
        case Verb::PONG:
            return "PONG";
        case Verb::DATA:
            return "DATA";
        case Verb::CONNECT:
            return "CONNECT";
        case Verb::CONNECTED:
            return "CONNECTED";
        case Verb::DISCONNECT:
            return "DISCONNECT";
        case Verb::GET:
            return "GET";
        case Verb::CHAT_SEND:
            return "CHAT_SEND";
        case Verb::CHAT_RECV:
            return "CHAT_RECV";
        case Verb::DATA_SEND:
            return "DATA_SEND";
        case Verb::DATA_RECV:
            return "DATA_RECV";
        case Verb::SCORE_SEND:
            return "SCORE_SEND";
        case Verb::SCORE_RECV:
            return "SCORE_RECV";
        case Verb::HIGHSCORE_RECV:
            return "HIGHSCORE_RECV";
        case Verb::QUIZ_QUESTION:
            return "QUIZ_QUESTION";
    }
    return "UNKNOWN";
}
