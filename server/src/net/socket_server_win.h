#ifdef VG_WIN

/**
 * Windows implentation of socket server.
 */

#pragma once

#include "socket_server.h"

namespace ME {

class SocketServerWin : public PlatformSocketServer {
   public:
    void Init() override;
    void Update(double deltaTime) override;
    void End() override;

    void SendPacket(Packet* packet, uint8_t clientID) override;

   private:
    int serverSocketFD;
};
}  // namespace ME

#endif  // VG_WIN
