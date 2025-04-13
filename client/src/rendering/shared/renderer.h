/**
 * Game Renderer
 */

#pragma once

#include <stdint.h>

#include <array>

#include "../../game/game.h"

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

/**
 * Renderering Interface for the game.
 * This will spawn the approprite renderer for the platform.
 * This will also handle the game loop and update the renderer.
 */
class Renderer {
   public:
    Renderer();
    ~Renderer();

    // Game control functions.

    // Init renderer.
    void Init();

    // Update renderer, mostly redraws.
    void Update();

    // Cleanup renderer
    void End();

   private:
    PlatformRenderer *platformRenderer;
};

}  // namespace ME
