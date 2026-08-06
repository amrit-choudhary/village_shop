#ifdef VG_WIN

/**
 * Game Engine side Main Entry Point for Windows.
 * Since Windows entry is a bit different, we have a simplified main entry point.
 */

#pragma once

#include <cassert>

#include "client/src/anim/animation_system.h"
#include "client/src/audio/audio_system.h"
#include "client/src/debug/debug_system.h"
#include "client/src/game/game.h"
#include "client/src/game/game_breakout.h"
#include "client/src/game/game_character_test.h"
#include "client/src/game/game_dice_simple.h"
#include "client/src/game/game_falling_sand.h"
#include "client/src/game/game_game_of_life.h"
#include "client/src/game/game_rpg.h"
#include "client/src/game/game_ui_demo.h"
#include "client/src/game/village_game.h"
#include "client/src/input/input_manager.h"
#include "client/src/input/input_manager_win.h"
#include "client/src/misc/global_vars.h"
#include "client/src/net/connection.h"
#include "client/src/rendering/directx/renderer_dx.h"
#include "client/src/ui/ui_system.h"
#include "shared/src/file_io/ini/ini_parser.h"
#include "logging/src/logging.h"
#include "shared/src/misc/utils.h"
#include "shared/src/physics/physics_system.h"
#include "shared/src/time/time_manager.h"

namespace ME {

class GameMain {
   public:
    GameMain();
    virtual ~GameMain();

    GameMain(const GameMain&) = delete;
    GameMain& operator=(const GameMain&) = delete;
    GameMain(GameMain&&) = delete;
    GameMain& operator=(GameMain&&) = delete;

    void Init(HWND hwnd);
    void Update();
    void Exit();
    void ShutDownGameSystems();

    /** This feeds the windows OS input events to the game.
     * Game will dispatch it to Input System. */
    void HandleInput(UINT msg, WPARAM wParam, LPARAM lParam);

   private:
    HWND hWnd;

    ME::Time::TimeManager timeManager;
    ME::Input::InputManager inputManager;
    ME::Input::InputManagerWin* winInputManager = nullptr;
    ME::Connection connection;
    // ME::GameBreakout game;
    // ME::GameOfLife game;
    // ME::GameFallingSand game;
    // ME::GameRPG game;
    // ME::GameDiceSimple game;
    ME::GameCharacterTest game;
    // ME::GameUIDemo game;
    ME::PhysicsSystem physicsSystem;
    ME::AnimationSystem animationSystem;
    ME::AudioSystem audioSystem;
    ME::UISystem uiSystem;
    ME::DebugSystem debugSystem;
    ME::RendererDX renderer;

    int fixedFrameRate = 0;
    bool vsync = true;
};

}  // namespace ME

#endif  // VG_WIN
