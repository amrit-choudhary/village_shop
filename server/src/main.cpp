#include <chrono>
#include <cstdlib>
#include <iostream>
#include <string>
#include <thread>

#ifdef VG_WIN
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#endif

#include "misc/global_vars.h"
#include "net/game_server.h"
#include "shared/src/file_io/ini/ini_parser.h"
#include "shared/src/misc/utils.h"
#include "shared/src/net/networking.h"
#include "shared/src/time/time_manager.h"
#include "shared/src/time/timer_manager.h"

int main(int argc, char** argv) {
#ifdef VG_WIN
    char exePath[MAX_PATH];
    GetModuleFileNameA(NULL, exePath, MAX_PATH);
    ME::Utils::SetPaths(exePath, nullptr);
#else
    ME::Utils::SetPaths(argv[0], nullptr);
#endif

    ME::Time::TimeManager timeManager;
    ME::Time::TimeConfig timeConfig;
    timeConfig.fixedStepFPS = ME::Time::FPS_60;
    timeManager.Init(timeConfig);

    ME::Time::TimerManager timerManager;
    timerManager.Init(timeConfig.fixedStepFPS);

    ME::INIMap iniMap = ME::INIParser::Load();
    std::string portStr = iniMap["settings"]["port"];
    uint16_t port = portStr.empty() ? 9310 : static_cast<uint16_t>(std::atoi(portStr.c_str()));

    // Once per process, before any socket is created.
    if (!ME::Net::InitNetworking()) {
        return 1;
    }

    ME::GameServer gameServer;
    gameServer.SetTimerManagerRef(&timerManager);
    if (!gameServer.Init(port)) {
        // Init can fail after opening the score database, so close whatever it did open.
        gameServer.End();
        ME::Net::ShutdownNetworking();
        return 1;
    }

    // Game Loop.
    while (ServerRunning) {
        timeManager.BeginFrame();

        int steps = timeManager.GetPendingFixedSteps();
        for (int i = 0; i < steps; ++i) {
            double fixedDeltaTime = timeManager.GetFixedDeltaTime();

            // First in the step, so timers scheduled during step N fire at the start of step N+1.
            timerManager.Tick(fixedDeltaTime);
            gameServer.Update(fixedDeltaTime);
        }

        // TODO: temporary fix to stop this loop busy-spinning a full core - TimeManager itself
        // no longer paces anything (client relies on vsync instead), and the server has no
        // vsync equivalent. Sleeping only when idle keeps CPU usage low without materially
        // delaying tick processing (1ms << the 16.67ms/60Hz tick period), but a better fix
        // (e.g. blocking on socket recv with a timeout, or a proper idle-wait primitive) should
        // replace this.
        if (steps == 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }

    gameServer.End();
    // After the server ends, so its own cleanup can still clear handles; drops any it left behind.
    timerManager.End();
    ME::Net::ShutdownNetworking();
    return 0;
}