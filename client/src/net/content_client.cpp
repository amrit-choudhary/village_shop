#include "content_client.h"

#include "logging/src/logging.h"
#include "shared/src/net/content_protocol.h"
#include "shared/src/serialization/byte_writer.h"

using ME::Net::ContentProtocol::Verb;
using ME::Net::TcpResult;

ME::ContentClient::ContentClient()
    : recvStorage(new uint8_t[Net::ContentProtocol::MAX_MESSAGE_SIZE]),
      receiver(recvStorage, Net::ContentProtocol::MAX_MESSAGE_SIZE, Net::ContentProtocol::MAX_MESSAGE_SIZE) {}

ME::ContentClient::~ContentClient() {
    End();
    delete[] recvStorage;
}

void ME::ContentClient::Init(const char* serverIP, uint16_t serverPort, double now) {
    if (!Net::TcpSocket::InitNetworking()) {
        Fail("networking unavailable");
        return;
    }
    networkingStarted = true;

    LogInfo("Content: connecting to ", serverIP, ":", serverPort);
    const TcpResult result = socket.Connect(serverIP, serverPort);
    if (result == TcpResult::Error) {
        Fail("connect failed");
        return;
    }

    if (result == TcpResult::WouldBlock) {
        SetState(ContentSyncState::Connecting, now);
        return;
    }

    // Connected immediately (possible on localhost).
    if (!QueueGetManifest()) {
        Fail("could not build GET_MANIFEST");
        return;
    }
    SetState(ContentSyncState::RequestingManifest, now);
}

void ME::ContentClient::Update(double now) {
    switch (state) {
        case ContentSyncState::Connecting:
            UpdateConnecting(now);
            break;
        case ContentSyncState::RequestingManifest:
            UpdateRequestingManifest(now);
            break;
        case ContentSyncState::Idle:
        case ContentSyncState::Done:
        case ContentSyncState::Failed:
            break;
    }
}

void ME::ContentClient::End() {
    socket.Close();
    if (networkingStarted) {
        Net::TcpSocket::ShutdownNetworking();
        networkingStarted = false;
    }
}

ME::ContentSyncState ME::ContentClient::GetState() const {
    return state;
}

void ME::ContentClient::UpdateConnecting(double now) {
    const TcpResult result = socket.PollConnect();
    if (result == TcpResult::Error) {
        Fail("connect failed");
        return;
    }
    if (result == TcpResult::WouldBlock) {
        if (now - stateStartTime > CONNECT_TIMEOUT_SECONDS) {
            Fail("connect timed out");
        }
        return;
    }

    LogInfo("Content: connected");
    if (!QueueGetManifest()) {
        Fail("could not build GET_MANIFEST");
        return;
    }
    SetState(ContentSyncState::RequestingManifest, now);
}

void ME::ContentClient::UpdateRequestingManifest(double now) {
    if (!FlushSend() || !ReceiveAvailable()) {
        return;
    }

    Net::FrameView message;
    const Net::FrameResult frame = receiver.PeekFrame(message);
    if (frame == Net::FrameResult::Invalid) {
        Fail("server sent an invalid frame");
        return;
    }
    if (frame == Net::FrameResult::Incomplete) {
        if (now - stateStartTime > REPLY_TIMEOUT_SECONDS) {
            Fail("timed out waiting for MANIFEST");
        }
        return;
    }

    // message.payload points into the receiver, so use it before PopFrame.
    const bool ok = HandleManifest(message);
    receiver.PopFrame();
    if (!ok) {
        return;
    }

    socket.Close();
    SetState(ContentSyncState::Done, now);
    LogSuccess("Content: sync step done (manifest received)");
}

void ME::ContentClient::SetState(ContentSyncState newState, double now) {
    state = newState;
    stateStartTime = now;
}

void ME::ContentClient::Fail(const char* reason) {
    LogWarning("Content: sync failed (", reason, "); continuing with existing content");
    socket.Close();
    state = ContentSyncState::Failed;
}

bool ME::ContentClient::QueueGetManifest() {
    ByteWriter writer(sendStorage, sizeof(sendStorage));
    size_t frameStart = 0;
    if (!Net::BeginFrame(writer, Net::ContentProtocol::VERSION, static_cast<uint8_t>(Verb::GET_MANIFEST),
                         frameStart) ||
        !Net::FinishFrame(writer, frameStart)) {
        return false;
    }
    sendSize = writer.GetSize();
    sendSent = 0;
    return true;
}

bool ME::ContentClient::FlushSend() {
    while (sendSent < sendSize) {
        int sent = 0;
        const TcpResult result = socket.Send(sendStorage + sendSent, static_cast<int>(sendSize - sendSent), sent);
        if (result == TcpResult::Error) {
            Fail("send failed");
            return false;
        }
        if (result == TcpResult::WouldBlock || sent == 0) {
            return true;
        }
        sendSent += static_cast<size_t>(sent);
    }
    return true;
}

bool ME::ContentClient::ReceiveAvailable() {
    // Read until nothing more is waiting this frame, so a large reply arrives as fast as the network allows.
    while (receiver.GetFreeSize() > 0) {
        int received = 0;
        const TcpResult result =
            socket.Recv(receiver.GetFreeData(), static_cast<int>(receiver.GetFreeSize()), received);
        if (result == TcpResult::Ok) {
            receiver.CommitReceived(static_cast<size_t>(received));
            continue;
        }
        if (result == TcpResult::WouldBlock) {
            return true;
        }
        Fail(result == TcpResult::Closed ? "server closed the connection" : "recv failed");
        return false;
    }
    return true;
}

bool ME::ContentClient::HandleManifest(const Net::FrameView& message) {
    if (message.version != Net::ContentProtocol::VERSION || message.verb != static_cast<uint8_t>(Verb::MANIFEST)) {
        Fail("expected a MANIFEST reply");
        return false;
    }
    if (!serverManifest.Parse(reinterpret_cast<const char*>(message.payload), message.payloadSize)) {
        Fail("invalid manifest");
        return false;
    }

    LogInfo("Content: server lists ", serverManifest.GetCount(), " files");
    for (uint32_t i = 0; i < serverManifest.GetCount(); ++i) {
        const Net::ManifestEntry& entry = serverManifest.GetEntry(i);
        LogInfo("Content:   ", entry.name, " v", entry.version);
    }
    return true;
}
