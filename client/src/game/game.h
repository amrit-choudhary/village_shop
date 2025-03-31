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

constexpr uint8_t BUFFER_X = 80;
constexpr uint8_t BUFFER_X_2 = 40;
constexpr uint8_t BUFFER_Y = 40;
constexpr uint8_t BUFFER_Y_2 = 20;
constexpr uint8_t ASCII_COUNT = 69;
constexpr uint8_t ASCII_COUNT_SHORT = 10;

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

    // Getters
    std::array<std::array<uint8_t, BUFFER_Y>, BUFFER_X> &GetBuffer() { return buffer1; }

    void SetInputManagerRef(ME::Input::InputManager *ptrInput);
    void SetConnectionRef(ME::Connection *ptrConnection);

   protected:
    ME::Time::TimeManager *timeManager;
    ME::Connection *connection;
    ME::Input::InputManager *inputManager;

    // Frame Buffer.
    std::array<std::array<uint8_t, BUFFER_Y>, BUFFER_X> buffer1;
};
}  // namespace ME