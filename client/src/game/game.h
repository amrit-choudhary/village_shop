/**
 * Main Game class.
 * Derive this class for your own game.
 */

#pragma once

#include <stdint.h>

#include <array>

#include "../input/input_manager.h"
#include "../net/connection.h"
#include "src/datastructure/ring_buffer.h"
#include "src/logging.h"
#include "src/math/math.h"
#include "src/random/random_engine.h"
#include "src/time/time_manager.h"
#include "villager.h"

namespace ME {

class Game {
   public:
    Game();
    ~Game();

    // Game control functions.

    // Init game
    virtual void Init(ME::Time::TimeManager *currentTimeManager);

    // Update game.
    virtual void Update(double deltaTime);

    // End game.
    virtual void End();

    void SetInputManagerRef(ME::Input::InputManager *ptrInput);
    void SetConnectionRef(ME::Connection *ptrConnection);

   protected:
    ME::Time::TimeManager *timeManager;
    ME::Connection *connection;
    ME::Input::InputManager *inputManager;
};
}  // namespace ME