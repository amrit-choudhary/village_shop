/**
 * This defines the protocol for communication between client and server.
 * Packet looks like this:
 * 1 byte : version number
 * 2 byte : Verbs
 * 1 byte : Client ID
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
    VER_0 = 0x00,
    VER_1 = 0x01,
    VER_2 = 0x02,
    VER_3 = 0x03,
    VER_4 = 0x04,
    VER_5 = 0x05,
    VER_6 = 0x06,
    VER_7 = 0x07,
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
    NONE = 0x00,
    ACK = 0x01,
    AUTH = 0x02,
    PING = 0x03,
    PONG = 0x04,
    DATA = 0x05,
    CONNECT = 0x06,
    CONNECTED = 0x07,
    DISCONNECT = 0x08,

    // Http
    GET = 0x20,

    // Gameplay
    CHAT_SEND = 0x60,
    CHAT_RECV = 0x61,

};

/**
 * Data representing a connected client.
 * Sever will have a list of ConnectedClients.
 */
class ConnectedClient {
   public:
    /**
     * Assigned by the server once CONNECT request comes.
     * Client will get the same ID and will send this in every request to the server.
     */
    uint8_t clientID;
    uint32_t address;
    uint16_t port;
};

/**
 * Data representing a connected server.
 * Used on client.
 */
class ConnectedServer {
   public:
    uint32_t address;
    uint16_t port;
    /**
     * ClientID as assigned by the serve. Send in every request from client to server.
     * Not connected = 0xFFFF.
     */
    uint8_t clientID = 0xFF;
};
}  // namespace ME::Net
