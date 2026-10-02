#include <atomic>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <thread>

#ifdef VG_WIN
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#endif

#include "logging/src/logging.h"
#include "shared/src/file_io/ini/ini_parser.h"
#include "shared/src/misc/utils.h"
#include "shared/src/net/content_protocol.h"
#include "shared/src/net/message_framing.h"
#include "shared/src/net/tcp_socket.h"
#include "shared/src/serialization/byte_reader.h"

namespace {
// Main loop flag. Nothing clears it yet; stop the process with Ctrl+C, like the game server.
std::atomic<bool> running(true);

constexpr uint16_t DEFAULT_PORT = 9311;

// Requests are small; any frame larger than this is rejected as invalid.
constexpr size_t RECV_CAPACITY = 4096;

void LogFrame(const ME::Net::FrameView& frame) {
    ME::LogInfo("Message: ", ME::Net::ContentProtocol::GetVerbName(frame.verb), ", ", frame.payloadSize, " payload bytes");

    if (frame.verb == static_cast<uint8_t>(ME::Net::ContentProtocol::Verb::GET_FILE)) {
        ME::ByteReader reader(frame.payload, frame.payloadSize);
        uint16_t pathLength = 0;
        const uint8_t* path = nullptr;
        if (reader.ReadU16(pathLength) && reader.ReadBytes(pathLength, path)) {
            ME::LogInfo("  path: ", std::string(reinterpret_cast<const char*>(path), pathLength));
        } else {
            ME::LogWarning("  malformed GET_FILE payload");
        }
    }
}

/**
 * Handles every complete frame in the receiver. Returns false if the connection should be closed.
 */
bool ProcessFrames(ME::Net::FrameReceiver& receiver) {
    ME::Net::FrameView frame;
    ME::Net::FrameResult result = receiver.PeekFrame(frame);
    while (result == ME::Net::FrameResult::Ok) {
        if (frame.version != ME::Net::ContentProtocol::VERSION) {
            ME::LogError("Unsupported protocol version ", static_cast<int>(frame.version));
            return false;
        }

        // frame.payload points into the receiver, so use it before PopFrame.
        LogFrame(frame);
        receiver.PopFrame();
        result = receiver.PeekFrame(frame);
    }

    if (result == ME::Net::FrameResult::Invalid) {
        ME::LogError("Invalid frame length");
        return false;
    }
    return true;
}
}  // namespace

int main(int argc, char** argv) {
#ifdef VG_WIN
    char exePath[MAX_PATH];
    GetModuleFileNameA(NULL, exePath, MAX_PATH);
    ME::Utils::SetPaths(exePath, nullptr);
#else
    ME::Utils::SetPaths(argv[0], nullptr);
#endif

    ME::INIMap iniMap = ME::INIParser::Load();
    const std::string portStr = iniMap["settings"]["port"];
    const uint16_t port = portStr.empty() ? DEFAULT_PORT : static_cast<uint16_t>(std::atoi(portStr.c_str()));

    ME::LogInfo("Content server starting on port ", port);
    ME::LogInfo("Serving from ", ME::Utils::GetDlcPath());

    if (!ME::Net::TcpSocket::InitNetworking()) {
        return 1;
    }

    ME::Net::TcpSocket listener;
    if (!listener.Listen(port)) {
        ME::Net::TcpSocket::ShutdownNetworking();
        return 1;
    }
    ME::LogSuccess("Listening on port ", port);

    // One client at a time for now; extra connections wait in the OS queue until this one leaves.
    ME::Net::TcpSocket client;
    uint8_t recvStorage[RECV_CAPACITY];
    ME::Net::FrameReceiver receiver(recvStorage, sizeof(recvStorage), ME::Net::ContentProtocol::MAX_MESSAGE_SIZE);

    while (running) {
        bool didWork = false;

        if (!client.IsOpen() && listener.Accept(client) == ME::Net::TcpResult::Ok) {
            ME::LogInfo("Client connected");
            receiver.Reset();
            didWork = true;
        }

        if (client.IsOpen()) {
            // Receive straight into the receiver's free space; it then splits the bytes into frames.
            int received = 0;
            const ME::Net::TcpResult result =
                client.Recv(receiver.GetFreeData(), static_cast<int>(receiver.GetFreeSize()), received);
            if (result == ME::Net::TcpResult::Ok) {
                ME::LogDebug("Received ", received, " bytes");
                receiver.CommitReceived(static_cast<size_t>(received));
                if (!ProcessFrames(receiver)) {
                    ME::LogError("Dropping client");
                    client.Close();
                }
                didWork = true;
            } else if (result == ME::Net::TcpResult::Closed) {
                ME::LogInfo("Client disconnected");
                client.Close();
                didWork = true;
            } else if (result == ME::Net::TcpResult::Error) {
                ME::LogError("Recv failed, dropping client");
                client.Close();
                didWork = true;
            }
        }

        // Sleep only when nothing happened, so the loop doesn't spin a full CPU core while idle.
        if (!didWork) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }

    client.Close();
    listener.Close();
    ME::Net::TcpSocket::ShutdownNetworking();
    return 0;
}
