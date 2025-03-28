#include <iostream>

#include "misc/global_vars.h"
#include "net/socket_server.h"
#include "src/time/time_manager.h"

int main(int argc, char** argv) {
    ME::Time::TimeManager timeManager;
    timeManager.Init(ME::Time::FPS_60);
    bool shouldTick = false;
    double deltaTime = 0.0f;

    ME::SocketServer socketServer;
    socketServer.Init();

    // Game Loop.
    while (ServerRunning) {
        shouldTick = timeManager.Update();

        // Game Tick.
        if (shouldTick) {
            deltaTime = timeManager.GetDeltaTime();

            socketServer.Update(deltaTime);
        }
    }

    return 0;
}