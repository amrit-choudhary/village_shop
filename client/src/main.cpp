#include <atomic>
#include <iostream>
#include <thread>

#include "file_io/ini/ini_parser.h"
#include "game/game.h"
#include "input/input_manager.h"
#include "misc/global_vars.h"
#include "rendering/renderer.h"
#include "src/logging.h"
#include "time/time_manager.h"

int main(int argc, char **argv) {
    // Read game params from file.
    INIMap iniMap = Load();
    int fps = std::atoi(iniMap["settings"]["fps"].c_str());
    int maxRunTime = std::atoi(iniMap["settings"]["maxRunTime"].c_str());

    // Init global variables.
    ME::Time::TimeManager timeManager;
    timeManager.Init(fps);
    bool shouldTick = false;
    double deltaTime = 0.0f;

    InputManager inputManager;
    inputManager.Init();
    ME::Game game;
    game.Init(&timeManager);
    Renderer renderer;
    renderer.Init();

    // Game Loop.
    while (GameRunning) {
        shouldTick = timeManager.Update();

        // Game Tick.
        if (shouldTick) {
            deltaTime = timeManager.GetDeltaTime();

            inputManager.Update(deltaTime);
            game.Update(deltaTime);
            // renderer.Update(game.GetBuffer());
        }

        if (timeManager.GetTimeSinceStartup() > maxRunTime) {
            GameRunning = false;
        }
    }

    // Game End.
    timeManager.End();
    inputManager.End();
    game.End();
    renderer.End();
}
