#ifdef VG_WIN

#include "socket_platform.h"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <winsock2.h>
#include <ws2tcpip.h>

#include "logging/src/logging.h"

namespace {
SOCKET ToNative(intptr_t s) {
    return static_cast<SOCKET>(s);
}
}  // namespace

bool ME::SocketPlatform::Init() {
    WSADATA wsaData;
    const int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        LogError("WSAStartup failed: ", result);
        return false;
    }
    return true;
}

void ME::SocketPlatform::Shutdown() {
    WSACleanup();
}

intptr_t ME::SocketPlatform::CreateTcp() {
    const SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    return s == INVALID_SOCKET ? INVALID_HANDLE : static_cast<intptr_t>(s);
}

void ME::SocketPlatform::Close(intptr_t s) {
    closesocket(ToNative(s));
}

bool ME::SocketPlatform::SetNonBlocking(intptr_t s) {
    u_long nonBlocking = 1;
    return ioctlsocket(ToNative(s), FIONBIO, &nonBlocking) == 0;
}

void ME::SocketPlatform::SetListenOptions(intptr_t s) {
    // Stop another program from binding the same port while we listen. (Windows' SO_REUSEADDR would allow it.)
    BOOL exclusive = TRUE;
    setsockopt(ToNative(s), SOL_SOCKET, SO_EXCLUSIVEADDRUSE, reinterpret_cast<const char*>(&exclusive),
               sizeof(exclusive));
}

void ME::SocketPlatform::SetConnectionOptions(intptr_t) {
    // Windows never raises SIGPIPE, so nothing to set.
}

bool ME::SocketPlatform::BindAny(intptr_t s, uint16_t port) {
    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    return bind(ToNative(s), reinterpret_cast<sockaddr*>(&address), sizeof(address)) == 0;
}

bool ME::SocketPlatform::Listen(intptr_t s) {
    return listen(ToNative(s), SOMAXCONN) == 0;
}

intptr_t ME::SocketPlatform::Accept(intptr_t s) {
    const SOCKET client = accept(ToNative(s), nullptr, nullptr);
    return client == INVALID_SOCKET ? INVALID_HANDLE : static_cast<intptr_t>(client);
}

int ME::SocketPlatform::Send(intptr_t s, const uint8_t* data, int size) {
    return send(ToNative(s), reinterpret_cast<const char*>(data), size, 0);
}

int ME::SocketPlatform::Recv(intptr_t s, uint8_t* buffer, int capacity) {
    return recv(ToNative(s), reinterpret_cast<char*>(buffer), capacity, 0);
}

int ME::SocketPlatform::LastError() {
    return WSAGetLastError();
}

bool ME::SocketPlatform::LastErrorIsWouldBlock() {
    return WSAGetLastError() == WSAEWOULDBLOCK;
}

#endif  // VG_WIN
