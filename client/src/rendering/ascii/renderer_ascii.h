/**
 * ASCII Renderer
 * This class is used to render the game using ASCII characters.
 */

#pragma once

#include "../renderer.h"

namespace ME {

/**
 * Renderer for ASCII graphics.
 */
class RendererASCII : public PlatformRenderer {
   public:
    // Init game
    void Init() override;

    // Will return true if game should tick based on FFR.
    void Update() override;

    // Will stop the timers to let it calculate average FPS.
    void End() override;

   private:
   protected:
};
}  // namespace ME
