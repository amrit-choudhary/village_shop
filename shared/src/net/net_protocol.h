/**
 * This defines the protocol for communication between client and server.
 * Packet looks like this:
 * 1 byte : version number
 * 2 byte : Verbs
 * x bytes: data
 */

#pragma once

#include <stdint.h>

namespace ME::Net {

/**
 * First byte of every packet.
 * Denoting the protocol version.
 */
enum class Version : uint8_t {
    VER_0 = 0x0000,
    VER_1 = 0x0001,
    VER_2 = 0x0002,
    VER_3 = 0x0003,
    VER_4 = 0x0004,
    VER_5 = 0x0005,
    VER_6 = 0x0006,
    VER_7 = 0x0007,
};

/**
 * Every packet sent between client and server will have these verbs that
 * will define the action taken on that packet.
 * In every packet Verb will be 2nd and 3rd byte. 2 byte long
 * Distribution:
 * 0x00 - 0x1F = System
 * 0x20 - 0x3F = http
 * 0x40 - 0x5F = matchmaking
 * 0x60 - 0x7F = gameplay
 * 0x80 - 0x9F
 * 0xA0 - 0xBF
 * 0xC0 - 0xDF
 * 0xE0 - 0xFF
 */
enum class Verb : uint8_t {
    // System
    NONE = 0x0000,
    ACK = 0x0001,
    AUTH = 0x0002,
    PING = 0x0003,
    PONG = 0x0004,
    DATA = 0x0005,

    // Http
    GET = 0x0020,

};
}  // namespace ME::Net