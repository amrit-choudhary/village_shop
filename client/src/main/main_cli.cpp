#ifdef VG_CLI

#include <atomic>
#include <iostream>
#include <thread>

#include "client/src/game/game.h"
#include "client/src/game/village_game.h"
#include "client/src/input/input_manager.h"
#include "client/src/misc/global_vars.h"
#include "client/src/net/connection.h"
#include "rendering/renderer.h"
#include "shared/src/file_io/ini/ini_parser.h"
#include "logging/src/logging.h"
#include "shared/src/misc/utils.h"
#include "shared/src/time/time_manager.h"

int main2(int argc, char **argv) {
    ME::SetPaths(argv[0], argv[1]);

    // Read game params from file.
    INIMap iniMap = Load();
    int fps = std::atoi(iniMap["settings"]["fps"].c_str());

    // Init global variables.
    ME::Time::TimeManager timeManager;
    timeManager.Init(fps);
    bool shouldTick = false;
    double deltaTime = 0.0f;

    ME::Input::InputManager inputManager;
    inputManager.Init();
    ME::Connection connection;
    connection.Init();

    ME::VillageGame game;
    game.SetInputManagerRef(&inputManager);
    game.SetConnectionRef(&connection);
    game.Init(&timeManager);

    ME::Renderer renderer;
    renderer.Init();

    // Game Loop.
    while (GameRunning) {
        shouldTick = timeManager.Update();

        // Game Tick.
        if (shouldTick) {
            deltaTime = timeManager.GetDeltaTime();

            inputManager.Update(deltaTime);
            game.Update(deltaTime);
            renderer.Update();
            connection.Update(deltaTime);
        }
    }

    // Game End.
    timeManager.End();
    // inputManager.End();
    game.End();
    renderer.End();
    connection.End();
}

#endif