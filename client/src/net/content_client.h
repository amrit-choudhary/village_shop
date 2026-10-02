/**
 * Talks to the content server in the background, one step per frame, never blocking the game.
 * Current steps: connect, request the manifest, log it. Downloading files comes next.
 */
#pragma once

#include <cstddef>
#include <cstdint>

#include "shared/src/net/content_manifest.h"
#include "shared/src/net/message_framing.h"
#include "shared/src/net/tcp_socket.h"

namespace ME {

enum class ContentSyncState : uint8_t {
    Idle,                // Init not called yet.
    Connecting,          // TCP handshake in progress.
    RequestingManifest,  // GET_MANIFEST sent, waiting for MANIFEST.
    Done,                // Finished; connection closed.
    Failed,              // Gave up (server down, timeout, bad reply); the game carries on without new content.
};

class ContentClient {
   public:
    ContentClient();
    ~ContentClient();

    // Owns a heap receive buffer that the receiver points into; copying would break both.
    ContentClient(const ContentClient&) = delete;
    ContentClient& operator=(const ContentClient&) = delete;

    /**
     * Starts connecting. now is the game's time in seconds (TimeManager::GetTimeSinceStartup).
     */
    void Init(const char* serverIP, uint16_t serverPort, double now);

    /**
     * Advances the sync by whatever is possible right now. Call once per frame.
     */
    void Update(double now);

    void End();

    ContentSyncState GetState() const;

   private:
    void UpdateConnecting(double now);
    void UpdateRequestingManifest(double now);

    void SetState(ContentSyncState newState, double now);
    void Fail(const char* reason);

    bool QueueGetManifest();
    bool FlushSend();

    /**
     * Receives whatever has arrived. Returns false (after calling Fail) if the connection broke.
     */
    bool ReceiveAvailable();

    bool HandleManifest(const Net::FrameView& message);

    // How long one step may take before giving up. The OS alone can take 20+ s to fail a connect.
    static constexpr double CONNECT_TIMEOUT_SECONDS = 5.0;
    static constexpr double REPLY_TIMEOUT_SECONDS = 10.0;
    static constexpr size_t SEND_CAPACITY = 512;

    ContentSyncState state = ContentSyncState::Idle;
    double stateStartTime = 0.0;
    bool networkingStarted = false;

    Net::TcpSocket socket;

    // Replies can be whole files (up to MAX_MESSAGE_SIZE), so the receive buffer is on the heap.
    uint8_t* recvStorage = nullptr;
    Net::FrameReceiver receiver;

    // Requests are small and sent from here; Send may take only part per call.
    uint8_t sendStorage[SEND_CAPACITY];
    size_t sendSize = 0;
    size_t sendSent = 0;

    Net::ContentManifest serverManifest;
};

}  // namespace ME
