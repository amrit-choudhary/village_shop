/**
 * Base class for making a socket server that can accept client connections.
 * Also has base class for platform dependent server implementation.
 */

#pragma once

#include <vector>

#include "src/net/net_packet.h"
#include "src/net/net_protocol.h"

namespace ME {

class SocketServer;  // Forward declaration.

class PlatformSocketServer {
   public:
    virtual void Init();
    virtual void Update(double deltaTime);
    virtual void End();

    virtual void SendPacket(Packet* packet);

    // Pointer to the server, which is the interface with other code.
    SocketServer* socketServer;

   protected:
   private:
};

class SocketServer {
   public:
    void Init();
    void Update(double deltaTime);
    void End();

    void ProcessPacket(Packet& packet, uint32_t fromAddr, uint16_t fromPort);
    void SendPong();
    void SendPacket(Packet* packet);
    ME::Net::ConnectedClient GetClient();

   private:
    PlatformSocketServer* platformSocketServer;
    std::vector<ME::Net::ConnectedClient> connectedClients;
};

}  // namespace ME