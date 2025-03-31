/**
 * Base class for making socket connection to the server.
 * Will also handle sending and receiving messages.
 */

#pragma once

#include <stdint.h>

#include "src/net/net_packet.h"
#include "src/net/net_protocol.h"

namespace ME {

class Connection;  // Forward declaration.

class PlatformConnection {
   public:
    virtual void Init();
    virtual void Update(double deltaTime);
    virtual void End();
    virtual void SendMessage(char* message);
    virtual void SendPacket(Packet* packet);

    // Pointer to the Connection, which is the interface with other code.
    Connection* connection;

   protected:
   private:
};

class Connection {
   public:
    void Init();
    void Update(double deltaTime);
    void End();
    void SendMessage(char* message);
    void SendConnectRequest();
    void SendPing();
    void SendChat(const char* message);
    void RecvChat(Packet& packet, uint8_t clientID);
    void SendPacket(Packet* packet);
    void ProcessPacket(Packet& packet, uint32_t fromAddr, uint16_t fromPort);
    uint8_t GetClientID();

   private:
    PlatformConnection* platformConnection;
    Net::ConnectedServer connectedServer;
};

}  // namespace ME