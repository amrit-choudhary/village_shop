/**
 * Base class for making a socket server that can accept client connections.
 * Also has base class for platform dependent server implementation.
 */

#pragma once

namespace ME {
class PlatformSocketServer {
   public:
    virtual void Init();
    virtual void Update(double deltaTime);
    virtual void End();

   protected:
   private:
};

class SocketServer {
   public:
    void Init();
    void Update(double deltaTime);
    void End();

   private:
    PlatformSocketServer* platformSocketServer;
};

}  // namespace ME