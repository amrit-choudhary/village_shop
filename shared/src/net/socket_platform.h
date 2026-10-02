/**
 * Thin per-platform wrappers over the OS socket API (Winsock on Windows, POSIX on Mac/Linux).
 * Internal to shared/src/net: socket classes call these so they stay free of #ifdefs.
 * Handles are intptr_t (SOCKET on Windows, file descriptor on POSIX); INVALID_HANDLE means no socket.
 */
#pragma once

#include <cstdint>

namespace ME::Net::SocketPlatform {

constexpr intptr_t INVALID_HANDLE = -1;

/**
 * Once per program, before / after any socket use. WSAStartup/WSACleanup on Windows, no-op on POSIX.
 */
bool Init();
void Shutdown();

intptr_t CreateTcp();
void Close(intptr_t s);
bool SetNonBlocking(intptr_t s);

/**
 * Listen socket options: exclusive port use on Windows, fast rebind after a restart on POSIX.
 */
void SetListenOptions(intptr_t s);

/**
 * Connected socket options: SIGPIPE protection on Mac; no-op elsewhere.
 */
void SetConnectionOptions(intptr_t s);

/**
 * Binds to port on all network interfaces.
 */
bool BindAny(intptr_t s, uint16_t port);
bool Listen(intptr_t s);

/**
 * Returns the new connection's handle, or INVALID_HANDLE (then check LastErrorIsWouldBlock).
 */
intptr_t Accept(intptr_t s);

/**
 * Return bytes sent / received, or -1 on failure (then check LastErrorIsWouldBlock).
 * Recv returns 0 when the other side closed the connection.
 */
int Send(intptr_t s, const uint8_t* data, int size);
int Recv(intptr_t s, uint8_t* buffer, int capacity);

/**
 * Error of the last failed socket call. Read it right after the failure, before other socket calls.
 */
int LastError();
bool LastErrorIsWouldBlock();

}  // namespace ME::Net::SocketPlatform
