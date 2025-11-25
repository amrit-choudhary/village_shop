/**
 * Game Renderer
 */

#pragma once

#include <cstdint>

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

    // Sets the scene for the renderer.
    // This will be called by the game to set the scene for the renderer.
    virtual void SetScene(ME::Scene *gameScene) = 0;

    // Sets both game and UI scenes for the renderer.
    virtual void SetScenes(ME::Scene *gameScene, ME::SceneUI *uiScene) = 0;
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
