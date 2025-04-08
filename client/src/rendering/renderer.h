/**
 * Game Renderer
 */

#pragma once

#include <stdint.h>

#include <array>

#include "../game/game.h"

namespace ME {

/**
 * Base interface for platform specific renderers.
 */
class PlatformRenderer {
   public:
    virtual void Init() = 0;
    virtual void Update() = 0;
    virtual void End() = 0;
};

class Renderer {
   public:
    Renderer();
    ~Renderer();

    // Game control functions.

    // Init game
    void Init();

    // Will return true if game should tick based on FFR.
    void Update();

    // Will stop the timers to let it calculate average FPS.
    void End();

   private:
    PlatformRenderer *platformRenderer;
};

}  // namespace ME