#ifdef VG_MAC

/**
 * Game Engine side Main Entry Point for macOS.
 * Since mac entry is a bit different, we have a simplified main entry point.
 */

#pragma once

#include <cassert>

#include "../game/game.h"
#include "../game/game_breakout.h"
#include "../game/village_game.h"
#include "../input/input_manager.h"
#include "../misc/global_vars.h"
#include "../net/connection.h"
// #include "../rendering/metal/renderer_metal.h"
#include "src/file_io/ini/ini_parser.h"
#include "src/logging.h"
#include "src/misc/utils.h"
#include "src/physics/physics_system.h"
#include "src/time/time_manager.h"

namespace ME {

// Forward declarations
// class MTK::View;
// class MTL::Device;

class GameMain {
   public:
    GameMain();
    virtual ~GameMain();

    // Sets Metal View and Device.
    // They are in objc land now, so we need to pass them in and convert.
    void SetViewAndDevice(void* view, void* device);
    void Init();
    void Update();
    void Exit();
    void ShutDownGameSystems();

   private:
    // MTK::View* mtkView;   // Pointer to the view it manages
    // MTL::Device* device;  // Pointer to the Metal device

    // Game Systems - Now owned by the view controller
    ME::Time::TimeManager timeManager;
    ME::Input::InputManager inputManager;  // Needs adaptation for NSEvent
    ME::Connection connection;
    ME::GameBreakout game;
    // ME::RendererMetal renderer;
    ME::PhysicsSystem physicsSystem;

    int fps = 0;
    int maxRunTime = 0;
};

}  // namespace ME

#endif  // VG_MAC
