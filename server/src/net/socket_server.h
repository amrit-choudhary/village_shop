/**
 * Base class for making a socket server that can accept client connections.
 * Also has base class for platform dependent server implementation.
 */

#pragma once

#include <vector>

#include "server/src/db/score_db.h"
#include "shared/src/net/game_protocol.h"
#include "shared/src/net/net_packet.h"

namespace ME {

/**
 * One client the server has accepted. clientID is assigned on CONNECT and sent back in every datagram.
 */
class ConnectedClient {
   public:
    uint8_t clientID;
    uint32_t address;
    uint16_t port;
};

class SocketServer;  // Forward declaration.

class PlatformSocketServer {
   public:
    virtual void Init(uint16_t port);
    virtual void Update(double deltaTime);
    virtual void End();

    virtual void SendPacket(Packet* packet, uint8_t clientID);

    // Pointer to the server, which is the interface with other code.
    SocketServer* socketServer;

   protected:
   private:
};

class SocketServer {
   public:
    void Init(uint16_t port);
    void Update(double deltaTime);
    void End();

    void ProcessPacket(Packet& packet, uint32_t fromAddr, uint16_t fromPort);
    void SendPacket(Packet* packet, uint8_t clientID);
    void SendPong(uint8_t clientID);
    void HandleChat(Packet& packet, uint8_t clientID);
    void HandleData(Packet& packet, uint8_t clientID);
    void HandleScore(Packet& packet, uint8_t clientID);
    void SendConnected(uint8_t clientID);
    void SendHighScore(uint8_t clientID);
    ME::ConnectedClient GetClient(uint8_t clientID);
    std::vector<ME::ConnectedClient> GetAllClients();

   private:
    PlatformSocketServer* platformSocketServer;
    std::vector<ME::ConnectedClient> connectedClients;
    ME::ScoreDB scoreDB;
};

}  // namespace ME