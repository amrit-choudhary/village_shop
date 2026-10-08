/**
 * Tests for the UDP game protocol encoding helpers.
 */

#include <cstdint>
#include <cstring>

#include "shared/src/math/fp_24_8.h"
#include "shared/src/net/game_protocol.h"
#include "shared/src/serialization/byte_reader.h"
#include "shared/src/serialization/byte_writer.h"
#include "test_framework/src/test_framework.h"

namespace GP = ME::Net::GameProtocol;

TEST(GameProtocol, HeaderRoundTrip) {
    uint8_t buffer[GP::MAX_DATAGRAM_SIZE] = {};
    ME::ByteWriter writer(buffer, sizeof(buffer));
    ASSERT(GP::WriteHeader(writer, GP::Verb::PING, 5));
    EXPECT(writer.GetSize() == 3);

    ME::ByteReader reader(buffer, writer.GetSize());
    GP::Header header;
    ASSERT(GP::ReadHeader(reader, header));
    EXPECT(header.version == GP::VERSION);
    EXPECT(header.verb == static_cast<uint8_t>(GP::Verb::PING));
    EXPECT(header.clientID == 5);
}

TEST(GameProtocol, ShortDatagramHasNoHeader) {
    const uint8_t data[2] = {0, 0x03};
    ME::ByteReader reader(data, sizeof(data));
    GP::Header header;
    header.clientID = 77;

    EXPECT(!GP::ReadHeader(reader, header));
    EXPECT(header.clientID == 77);
}

TEST(GameProtocol, FixedPointRoundTripIsExact) {
    uint8_t buffer[8] = {};
    ME::ByteWriter writer(buffer, sizeof(buffer));
    ME::FP_24_8 value(-12.75f);
    ASSERT(GP::WriteFP(writer, value));

    ME::ByteReader reader(buffer, writer.GetSize());
    ME::FP_24_8 out;
    ASSERT(GP::ReadFP(reader, out));
    EXPECT(out.GetRaw() == value.GetRaw());
}

TEST(GameProtocol, StringRoundTrip) {
    uint8_t buffer[64] = {};
    ME::ByteWriter writer(buffer, sizeof(buffer));
    ASSERT(GP::WriteString(writer, "hello"));
    EXPECT(writer.GetSize() == 6);

    ME::ByteReader reader(buffer, writer.GetSize());
    char out[GP::CHAT_CAPACITY] = {};
    ASSERT(GP::ReadString(reader, out, sizeof(out)));
    EXPECT(std::strcmp(out, "hello") == 0);
}

TEST(GameProtocol, StringTooBigForCapacityIsRejected) {
    uint8_t buffer[64] = {};
    ME::ByteWriter writer(buffer, sizeof(buffer));
    GP::WriteString(writer, "hello");

    ME::ByteReader reader(buffer, writer.GetSize());
    char out[5] = {'x', 0, 0, 0, 0};
    // 5 characters + terminating 0 needs 6 bytes.
    EXPECT(!GP::ReadString(reader, out, sizeof(out)));
    EXPECT(out[0] == 'x');
}

TEST(GameProtocol, TruncatedStringIsRejected) {
    const uint8_t data[3] = {10, 'a', 'b'};
    ME::ByteReader reader(data, sizeof(data));
    char out[32] = {};

    EXPECT(!GP::ReadString(reader, out, sizeof(out)));
}

TEST(GameProtocol, VerbNames) {
    EXPECT(std::strcmp(GP::GetVerbName(static_cast<uint8_t>(GP::Verb::PING)), "PING") == 0);
    EXPECT(std::strcmp(GP::GetVerbName(0xFF), "UNKNOWN") == 0);
}

TEST(GameProtocol, QuizQuestionRoundTrip) {
    GP::QuizQuestion question;
    question.id = 42;
    question.lhs = 9;
    question.rhs = 4;
    question.op = '-';
    question.options[0] = 6;
    question.options[1] = 5;

    uint8_t buffer[64] = {};
    ME::ByteWriter writer(buffer, sizeof(buffer));
    ASSERT(GP::WriteQuizQuestion(writer, question));
    EXPECT(writer.GetSize() == 9);

    ME::ByteReader reader(buffer, writer.GetSize());
    GP::QuizQuestion out;
    ASSERT(GP::ReadQuizQuestion(reader, out));
    EXPECT(out.id == 42);
    EXPECT(out.lhs == 9);
    EXPECT(out.rhs == 4);
    EXPECT(out.op == '-');
    EXPECT(out.options[0] == 6);
    EXPECT(out.options[1] == 5);
}

TEST(GameProtocol, QuizQuestionWithUnknownOpIsRejected) {
    GP::QuizQuestion question;
    question.id = 7;
    question.op = '*';

    uint8_t buffer[64] = {};
    ME::ByteWriter writer(buffer, sizeof(buffer));
    ASSERT(GP::WriteQuizQuestion(writer, question));

    ME::ByteReader reader(buffer, writer.GetSize());
    GP::QuizQuestion out;
    EXPECT(!GP::ReadQuizQuestion(reader, out));
    EXPECT(out.id == 0);
}

TEST(GameProtocol, TruncatedQuizQuestionIsRejected) {
    const uint8_t data[5] = {1, 0, 0, 0, 2};
    ME::ByteReader reader(data, sizeof(data));
    GP::QuizQuestion out;

    EXPECT(!GP::ReadQuizQuestion(reader, out));
}
