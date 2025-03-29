/**
 * Mac implentation of UDP socket server.
 */

#pragma once

#include "socket_server.h"

namespace ME {

class SocketServerMac : public PlatformSocketServer {
   public:
    void Init() override;
    void Update(double deltaTime) override;
    void End() override;

    void SendPacket(Packet* packet) override;

   private:
    int serverSockerFd;
};
}  // namespace ME