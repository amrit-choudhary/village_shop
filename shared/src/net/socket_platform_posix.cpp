#if defined(VG_LINUX) || defined(VG_MAC)

#include "socket_platform.h"

#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

namespace {
int ToNative(intptr_t s) {
    return static_cast<int>(s);
}
}  // namespace

bool ME::SocketPlatform::Init() {
    // POSIX sockets need no global setup.
    return true;
}

void ME::SocketPlatform::Shutdown() {}

intptr_t ME::SocketPlatform::CreateTcp() {
    const int s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    return s == -1 ? INVALID_HANDLE : static_cast<intptr_t>(s);
}

void ME::SocketPlatform::Close(intptr_t s) {
    close(ToNative(s));
}

bool ME::SocketPlatform::SetNonBlocking(intptr_t s) {
    // Read the current flags and add O_NONBLOCK, so other flags are kept.
    const int flags = fcntl(ToNative(s), F_GETFL, 0);
    return flags != -1 && fcntl(ToNative(s), F_SETFL, flags | O_NONBLOCK) != -1;
}

void ME::SocketPlatform::SetListenOptions(intptr_t s) {
    // Let a restarted server bind the port again right away, instead of waiting for old connections to expire.
    int reuse = 1;
    setsockopt(ToNative(s), SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
}

void ME::SocketPlatform::SetConnectionOptions(intptr_t s) {
#ifdef VG_MAC
    // Sending to a closed connection raises SIGPIPE, which kills the process; this makes it a normal error.
    int noSigPipe = 1;
    setsockopt(ToNative(s), SOL_SOCKET, SO_NOSIGPIPE, &noSigPipe, sizeof(noSigPipe));
#else
    // Linux has no SO_NOSIGPIPE; Send uses MSG_NOSIGNAL instead.
    (void)s;
#endif
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
    const int client = accept(ToNative(s), nullptr, nullptr);
    return client == -1 ? INVALID_HANDLE : static_cast<intptr_t>(client);
}

int ME::SocketPlatform::Send(intptr_t s, const uint8_t* data, int size) {
#ifdef VG_LINUX
    // Sending to a closed connection raises SIGPIPE, which kills the process; MSG_NOSIGNAL makes it a normal error.
    const int flags = MSG_NOSIGNAL;
#else
    const int flags = 0;  // Mac: handled by SO_NOSIGPIPE in SetConnectionOptions.
#endif
    return static_cast<int>(send(ToNative(s), data, static_cast<size_t>(size), flags));
}

int ME::SocketPlatform::Recv(intptr_t s, uint8_t* buffer, int capacity) {
    return static_cast<int>(recv(ToNative(s), buffer, static_cast<size_t>(capacity), 0));
}

int ME::SocketPlatform::LastError() {
    return errno;
}

bool ME::SocketPlatform::LastErrorIsWouldBlock() {
    return errno == EAGAIN || errno == EWOULDBLOCK;
}

#endif  // VG_LINUX || VG_MAC
