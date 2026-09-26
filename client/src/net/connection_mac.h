#ifdef VG_MAC
/**
 * Mac implementation for socket for client side.
 */

#pragma once

#include "connection.h"

namespace ME {

class ConnectionMac : public PlatformConnection {
   public:
    void Init(const char* serverIP, uint16_t serverPort) override;
    void Update(double deltaTime) override;
    void End() override;
    void SendPacket(Packet* packet) override;

   private:
    int clientSocketFD;
    char serverIP[64];
    uint16_t serverPort;
};
}  // namespace ME

#endif  // VG_MAC
