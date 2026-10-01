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

    while (running) {
        // Sockets come in a later step. Sleep so the empty loop doesn't spin a full CPU core.
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    return 0;
}
