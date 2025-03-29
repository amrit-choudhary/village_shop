/**
 * Mac implementation for socket for client side.
 */

#pragma once

#include "connection.h"

namespace ME {

class ConnectionMac : public PlatformConnection {
   public:
    void Init() override;
    void Update(double deltaTime) override;
    void End() override;
    void SendMessage(char* message) override;
    void SendPacket(Packet* packet) override;

   private:
    int clientSockerFd;
};
}  // namespace ME