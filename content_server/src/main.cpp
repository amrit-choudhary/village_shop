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
#include "shared/src/time/time_manager.h"

#include "client_connection.h"
#include "content_store.h"

namespace {
// Main loop flag. Nothing clears it yet; stop the process with Ctrl+C, like the game server.
std::atomic<bool> running(true);

constexpr uint16_t DEFAULT_PORT = 9311;

constexpr uint32_t MAX_CLIENTS = 32;

// Static storage: 32 connections with their buffers are ~150 KB, too much to put on the stack comfortably.
ME::ClientConnection clients[MAX_CLIENTS];

/**
 * Accepts every waiting connection into a free slot. When all slots are taken, extra connections are
 * accepted and closed immediately so they fail fast instead of waiting in the OS queue.
 */
bool AcceptNewClients(ME::Net::TcpSocket& listener, double now) {
    bool didWork = false;
    while (true) {
        uint32_t freeSlot = MAX_CLIENTS;
        for (uint32_t i = 0; i < MAX_CLIENTS; ++i) {
            if (!clients[i].IsOpen()) {
                freeSlot = i;
                break;
            }
        }

        if (freeSlot < MAX_CLIENTS) {
            if (!clients[freeSlot].AcceptFrom(listener, freeSlot, now)) {
                return didWork;
            }
        } else {
            ME::Net::TcpSocket rejected;
            if (listener.Accept(rejected) != ME::Net::TcpResult::Ok) {
                return didWork;
            }
            ME::LogWarning("Server full (", MAX_CLIENTS, " clients), rejecting connection");
            rejected.Close();
        }
        didWork = true;
    }
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

    // Only used as a clock (GetTimeSinceStartup); the server has no fixed-step simulation.
    ME::Time::TimeManager timeManager;
    timeManager.Init(ME::Time::TimeConfig{});

    while (running) {
        timeManager.BeginFrame();
        const double now = timeManager.GetTimeSinceStartup();
        bool didWork = AcceptNewClients(listener, now);

        for (ME::ClientConnection& client : clients) {
            if (client.IsOpen() && client.Update(store, now)) {
                didWork = true;
            }
        }

        // Sleep only when nothing happened, so the loop doesn't spin a full CPU core while idle.
        if (!didWork) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }

    for (ME::ClientConnection& client : clients) {
        client.Close();
    }
    listener.Close();
    ME::Net::TcpSocket::ShutdownNetworking();
    return 0;
}
