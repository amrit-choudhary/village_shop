#include <atomic>
#include <iostream>
#include <thread>

#include "game/game.h"
#include "game/village_game.h"
#include "input/input_manager.h"
#include "misc/global_vars.h"
#include "net/connection.h"
#include "rendering/renderer.h"
#include "src/file_io/ini/ini_parser.h"
#include "src/logging.h"
#include "src/misc/utils.h"
#include "src/time/time_manager.h"

int main(int argc, char **argv) {
    ME::SetPaths(argv[0], argv[1]);

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
    // inputManager.Init();
    ME::Connection connection;
    connection.Init();

    ME::VillageGame game;
    game.SetConnectionRef(&connection);
    game.Init(&timeManager);

    Renderer renderer;
    renderer.Init();

    // Game Loop.
    while (GameRunning) {
        shouldTick = timeManager.Update();

        // Game Tick.
        if (shouldTick) {
            deltaTime = timeManager.GetDeltaTime();

            // inputManager.Update(deltaTime);
            game.Update(deltaTime);
            // renderer.Update(game.GetBuffer());
            connection.Update(deltaTime);
        }

        if (timeManager.GetTimeSinceStartup() > maxRunTime) {
            GameRunning = false;
        }
    }

    // Game End.
    timeManager.End();
    // inputManager.End();
    game.End();
    renderer.End();
    connection.End();
}
