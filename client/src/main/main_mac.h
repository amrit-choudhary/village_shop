#ifdef VG_MAC

/**
 * Game Engine side Main Entry Point for macOS.
 * Since mac entry is a bit different, we have a simplified main entry point.
 */

#pragma once

#include <cassert>

#include "../game/game.h"
#include "../game/game_breakout.h"
#include "../game/game_ui_demo.h"
#include "../game/village_game.h"
#include "../input/input_manager.h"
#include "../input/input_manager_mac.h"
#include "../misc/global_vars.h"
#include "../net/connection.h"
#include "../rendering/metal/renderer_metal.h"
#include "../ui/ui_system.h"
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

    // Receives the Metal device/view once at boot, mirroring GameMain::Init(HWND) on Windows.
    void Init(MTL::Device* device, MTK::View* view);
    void Update();
    void Exit();
    void ShutDownGameSystems();

    /** Feeds macOS input events to the game. Game dispatches it to the Input System. */
    void HandleKeyEvent(uint16_t keyCode, bool isDown);
    void HandleMouseMove(float x, float y);
    void HandleMouseButton(int button, bool isDown);

   private:
    // Game Systems
    ME::Time::TimeManager timeManager;
    ME::Input::InputManager inputManager;
    ME::Input::InputManagerMac* macInputManager = nullptr;
    ME::Connection connection;
    // ME::GameBreakout game;
    ME::GameUIDemo game;
    ME::RendererMetal renderer;
    ME::PhysicsSystem physicsSystem;
    ME::UISystem uiSystem;

    int fps = 0;
    int maxRunTime = 0;
};

}  // namespace ME

#endif  // VG_MAC
