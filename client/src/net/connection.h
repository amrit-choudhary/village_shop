/**
 * Base class for making socket connection to the server.
 * Will also handle sending and receiving messages.
 */

#pragma once

#include <stdint.h>

#include "src/net/net_packet.h"
#include "src/net/net_protocol.h"

namespace ME {

class PlatformConnection {
   public:
    virtual void Init();
    virtual void Update(double deltaTime);
    virtual void End();
    virtual void SendMessage(char* message);
    virtual void SendPacket(Packet* packet);

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
    void SendPacket(Packet* packet);

   private:
    PlatformConnection* platformConnection;
    Net::ConnectedServer connectedServer;
};

}  // namespace ME