#include "connection.h"
#ifdef VG_MAC
#include "connection_mac.h"
#endif

void ME::Connection::Init() {
#ifdef VG_MAC
    platformConnection = new ME::ConnectionMac();
#endif
    platformConnection->Init();
}

void ME::Connection::Update(double deltaTime) { platformConnection->Update(deltaTime); }

void ME::Connection::End() { platformConnection->End(); }

void ME::Connection::SendMessage(char* message) { platformConnection->SendMessage(message); }

void ME::PlatformConnection::Init() {}

void ME::PlatformConnection::Update(double deltaTime) {}

void ME::PlatformConnection::End() {}

void ME::PlatformConnection::SendMessage(char* message) {}
