/**
 * ASCII Renderer
 * This class is used to render the game using ASCII characters.
 */

#pragma once

#include "../shared/renderer.h"

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
    static const uint8_t BUFFER_X = 80;
    static const uint8_t BUFFER_X_2 = 40;
    static const uint8_t BUFFER_Y = 40;
    static const uint8_t BUFFER_Y_2 = 20;
    static const uint8_t ASCII_COUNT = 69;
    static const uint8_t ASCII_COUNT_SHORT = 10;

    std::array<std::array<uint8_t, BUFFER_Y>, BUFFER_X> frameBuffer;

   protected:
};
}  // namespace ME
