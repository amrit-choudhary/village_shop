#include <chrono>
#include <iostream>
#include <thread>

#include "misc/global_vars.h"
#include "net/socket_server.h"
#include "src/time/time_manager.h"

int main(int argc, char** argv) {
    ME::Time::TimeManager timeManager;
    ME::Time::TimeConfig timeConfig;
    timeConfig.fixedStepFPS = ME::Time::FPS_60;
    timeManager.Init(timeConfig);

    ME::SocketServer socketServer;
    socketServer.Init();

    // Game Loop.
    while (ServerRunning) {
        timeManager.BeginFrame();

        int steps = timeManager.GetPendingFixedSteps();
        for (int i = 0; i < steps; ++i) {
            socketServer.Update(timeManager.GetFixedDeltaTime());
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

    return 0;
}