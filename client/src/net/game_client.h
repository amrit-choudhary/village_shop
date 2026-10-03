/**
 * Client side of the UDP game connection: talks to one game server, sends requests and turns replies into
 * delegate calls. Wire format in shared/src/net/game_protocol.h; socket in shared/src/net/udp_socket.h.
 */

#pragma once

#include <cstddef>
#include <cstdint>

#include "client/src/misc/delegate.h"
#include "shared/src/math/fp_24_8.h"
#include "shared/src/net/net_address.h"
#include "shared/src/net/udp_socket.h"

namespace ME {

class ByteReader;

/**
 * The game server this client talks to. clientID is assigned by the server on CONNECT; 0xFF = not connected.
 */
class ConnectedServer {
   public:
    Net::Address address;
    uint8_t clientID = 0xFF;
};

class GameClient {
   public:
    /**
     * Opens a UDP socket (OS-chosen port) for talking to the server. False if the address or socket is bad.
     */
    bool Init(const char* serverIP = "127.0.0.1", uint16_t serverPort = 9310);

    /**
     * Handles every datagram that has arrived since the last call. Call once per frame.
     */
    void Update(double deltaTime);
    void End();

    void SendConnectRequest();
    void SendPing();
    void SendChat(const char* message);
    void SendGameData(const FP_24_8& value1, const FP_24_8& value2, const FP_24_8& value3);
    void SendScore(uint32_t score);

    uint8_t GetClientID() const;
    bool IsConnected() const;
    uint8_t GetLastScoreSenderID() const;
    uint32_t GetLastScore() const;
    uint32_t GetHighScore() const;

    Delegate onConnected;
    Delegate onPong;
    Delegate onScoreReceived;
    Delegate onHighScoreReceived;

   private:
    void ProcessDatagram(const uint8_t* data, int size);

    void RecvChat(ByteReader& reader, uint8_t clientID);
    void RecvGameData(ByteReader& reader);
    void RecvScore(ByteReader& reader, uint8_t clientID);
    void RecvHighScore(ByteReader& reader);

    void SendDatagram(const uint8_t* data, size_t size);

    // Upper bound per Update, so a flood of datagrams can't stall a frame.
    static constexpr int MAX_DATAGRAMS_PER_UPDATE = 256;

    Net::UdpSocket socket;
    ConnectedServer connectedServer;

    uint8_t lastScoreSenderID = 0;
    uint32_t lastScore = 0;
    uint32_t highScore = 0;
};

}  // namespace ME
