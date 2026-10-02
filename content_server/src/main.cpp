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
#include "shared/src/net/tcp_socket.h"

#include "client_connection.h"
#include "content_store.h"

namespace {
// Main loop flag. Nothing clears it yet; stop the process with Ctrl+C, like the game server.
std::atomic<bool> running(true);

constexpr uint16_t DEFAULT_PORT = 9311;
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

    // Load all content before accepting clients; it stays unchanged until the server restarts.
    ME::ContentStore store;
    if (!store.Load()) {
        return 1;
    }

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
    ME::ClientConnection client;

    while (running) {
        bool didWork = false;

        if (!client.IsOpen() && client.AcceptFrom(listener)) {
            ME::LogInfo("Client connected");
            didWork = true;
        }

        if (client.IsOpen() && client.Update(store)) {
            didWork = true;
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
