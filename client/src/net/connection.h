/**
 * Base class for making socket connection to the server.
 * Will also handle sending and receiving messages.
 */

#pragma once

#include <cstdint>

#include "client/src/misc/delegate.h"
#include "shared/src/net/net_packet.h"
#include "shared/src/net/net_protocol.h"

namespace ME {

class Connection;  // Forward declaration.

class PlatformConnection {
   public:
    virtual void Init(const char* serverIP, uint16_t serverPort);
    virtual void Update(double deltaTime);
    virtual void End();
    virtual void SendPacket(Packet* packet);

    // Pointer to the Connection, which is the interface with other code.
    Connection* connection;

   protected:
   private:
};

class Connection {
   public:
    void Init(const char* serverIP = "127.0.0.1", uint16_t serverPort = 9310);
    void Update(double deltaTime);
    void End();
    void SendConnectRequest();
    void SendPing();
    void SendChat(const char* message);
    void RecvChat(Packet& packet, uint8_t clientID);
    void SendGameData(const ME::FP_24_8& value1, const ME::FP_24_8& value2, const ME::FP_24_8& value3);
    void RecvGameData(Packet& packet, uint8_t clientID);
    void SendScore(uint32_t score);
    void RecvScore(Packet& packet, uint8_t clientID);
    void RecvHighScore(Packet& packet);
    void SendPacket(Packet* packet);
    void ProcessPacket(Packet& packet, uint32_t fromAddr, uint16_t fromPort);
    uint8_t GetClientID();
    bool IsConnected() const;
    uint8_t GetLastScoreSenderID() const;
    uint32_t GetLastScore() const;
    uint32_t GetHighScore() const;

    Delegate onConnected;
    Delegate onPong;
    Delegate onScoreReceived;
    Delegate onHighScoreReceived;

   private:
    PlatformConnection* platformConnection;
    Net::ConnectedServer connectedServer;

    uint8_t lastScoreSenderID = 0;
    uint32_t lastScore = 0;
    uint32_t highScore = 0;
};

}  // namespace ME