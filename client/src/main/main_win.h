#ifdef VG_WIN

/**
 * Game Engine side Main Entry Point for Windows.
 * Since Windows entry is a bit different, we have a simplified main entry point.
 */

#pragma once

#include <cassert>

#include "../game/game.h"
#include "../game/game_breakout.h"
#include "../game/game_falling_sand.h"
#include "../game/game_game_of_life.h"
#include "../game/game_rpg.h"
#include "../game/village_game.h"
#include "../input/input_manager.h"
#include "../misc/global_vars.h"
#include "../net/connection.h"
#include "../rendering/directx/renderer_dx.h"
#include "src/file_io/ini/ini_parser.h"
#include "src/logging.h"
#include "src/misc/utils.h"
#include "src/physics/physics_system.h"
#include "src/time/time_manager.h"

namespace ME {

class GameMain {
   public:
    GameMain();
    virtual ~GameMain();

    void Init(HWND hwnd);
    void Update();
    void Exit();
    void ShutDownGameSystems();

   private:
    HWND hWnd;

    ME::Time::TimeManager timeManager;
    ME::Input::InputManager inputManager;
    ME::Connection connection;
    // ME::GameBreakout game;
    // ME::GameOfLife game;
    // ME::GameFallingSand game;
    ME::GameRPG game;
    ME::PhysicsSystem physicsSystem;
    ME::RendererDX renderer;

    int fps = 0;
    int maxRunTime = 0;
};

}  // namespace ME

#endif  // VG_WIN
