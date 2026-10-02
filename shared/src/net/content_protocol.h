/**
 * Wire protocol between the content server and its clients (over TCP).
 * Every message: u32 length | u8 version | u8 verb | payload. length counts the bytes after itself.
 * Integers are in native byte order (all supported targets are little-endian).
 */
#pragma once

#include <cstdint>

namespace ME::ContentProtocol {

constexpr uint8_t VERSION = 1;

// Bytes before the payload: u32 length + u8 version + u8 verb.
constexpr uint32_t HEADER_SIZE = 4 + 1 + 1;

// Largest allowed message (header included). Bigger messages are rejected.
constexpr uint32_t MAX_MESSAGE_SIZE = 1024 * 1024;

enum class Verb : uint8_t {
    // Client -> server.
    GET_MANIFEST = 0x01,  // No payload.
    GET_FILE = 0x02,      // u16 pathLen, path, u32 rangeOffset, u32 rangeLength (range reserved: always 0, 0).

    // Server -> client.
    MANIFEST = 0x81,        // Raw manifest.json bytes.
    FILE = 0x82,            // u16 pathLen, path, file bytes (size = rest of the message).
    FILE_NOT_FOUND = 0x83,  // u16 pathLen, path.
};

}  // namespace ME::ContentProtocol
