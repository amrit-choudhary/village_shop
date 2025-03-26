/**
 * Mac implentation of socket server.
 */

#pragma once

#include "socket_server.h"

namespace ME {

class SocketServerMac : public PlatformSocketServer {
   public:
    void Init() override;
    void Update(double deltaTime) override;
    void End() override;

   private:
};
}  // namespace ME