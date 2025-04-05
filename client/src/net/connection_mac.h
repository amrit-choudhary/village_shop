#ifdef VG_MAC
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
    void SendPacket(Packet* packet) override;

   private:
    int clientSocketFD;
};
}  // namespace ME

#endif  // VG_MAC
