/**
 * Main Game renderer
 */

#pragma once

#include <stdint.h>

#include <array>

#include "../game/game.h"

class Renderer {
   public:
    Renderer();
    ~Renderer();

    // Game control functions.

    // Init game
    void Init();

    // Will return true if game should tick based on FFR.
    void Update(std::array<std::array<uint8_t, ME::BUFFER_Y>, ME::BUFFER_X> &frameBuffer);

    // Will stop the timers to let it calculate average FPS.
    void End();

   private:
};