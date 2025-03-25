/**
 * Base class for making socket connection to the server.
 * Will also handle sending and receiving messages.
 */

#pragma once

namespace ME {

class PlatformConnection {
   public:
    virtual void Init();
    virtual void Update(double deltaTime);
    virtual void End();
    virtual void SendMessage(char* message);

   protected:
   private:
};

class Connection {
   public:
    void Init();
    void Update(double deltaTime);
    void End();
    void SendMessage(char* message);

   private:
    PlatformConnection* platformConnection;
};

}  // namespace ME