#include "networking.h"

#include "shared/src/net/socket_platform.h"

bool ME::Net::InitNetworking() {
    return SocketPlatform::Init();
}

void ME::Net::ShutdownNetworking() {
    SocketPlatform::Shutdown();
}
