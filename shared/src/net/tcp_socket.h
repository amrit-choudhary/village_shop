/**
 * Non-blocking TCP socket shared by servers and clients. A server uses Listen + Accept, a client
 * uses Connect; after that both ends use the same Send/Recv/Close. OS calls go through SocketPlatform.
 */
#pragma once

#include <cstdint>

namespace ME::Net {

enum class TcpResult : uint8_t {
    Ok,          // Call did its work (Send may still have sent only part of the data).
    WouldBlock,  // Nothing to do right now (no data, no pending connection, send buffer full). Try again later.
    Closed,      // The other side closed the connection.
    Error,       // Connection is broken; close this socket.
};

class TcpSocket {
   public:
    TcpSocket() = default;
    ~TcpSocket();

    // A socket owns an OS handle; copying would close it twice.
    TcpSocket(const TcpSocket&) = delete;
    TcpSocket& operator=(const TcpSocket&) = delete;

    /**
     * Call once at program start / end before using any socket. Needed by Winsock; no-op on POSIX.
     */
    static bool InitNetworking();
    static void ShutdownNetworking();

    /**
     * Server: start listening for connections on port (all network interfaces).
     */
    bool Listen(uint16_t port);

    /**
     * Server: take one pending connection into outClient. WouldBlock if nobody is waiting.
     */
    TcpResult Accept(TcpSocket& outClient);

    /**
     * Sends up to size bytes; outSent says how many were actually sent (can be fewer).
     */
    TcpResult Send(const uint8_t* data, int size, int& outSent);

    /**
     * Receives up to capacity bytes into buffer; outReceived says how many arrived.
     */
    TcpResult Recv(uint8_t* buffer, int capacity, int& outReceived);

    void Close();
    bool IsOpen() const;

   private:
    // OS socket handle: SOCKET on Windows, int file descriptor on POSIX. -1 (SocketPlatform::INVALID_HANDLE) = none.
    intptr_t handle = -1;
};

}  // namespace ME::Net
