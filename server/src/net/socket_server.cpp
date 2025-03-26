#include "socket_server.h"
#ifdef VG_MAC
#include "socket_server_mac.h"
#endif

void ME::SocketServer::Init() {
#ifdef VG_MAC
    platformSocketServer = new ME::SocketServerMac();
#endif
    platformSocketServer->Init();
}
void ME::SocketServer::Update(double deltaTime) { platformSocketServer->Update(deltaTime); }

void ME::SocketServer::End() { platformSocketServer->End(); }

void ME::PlatformSocketServer::Init() {}

void ME::PlatformSocketServer::Update(double deltaTime) {}

void ME::PlatformSocketServer::End() {}
