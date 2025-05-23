/**
 * This is a simple sprite renderer class.
 * It is used to render 2D sprites in the game.
 */

#pragma once

#include <cstdint>
#include <string>

#include "color.h"
#include "transform.h"

namespace ME {

class SpriteRenderer {
   public:
    SpriteRenderer() = delete;
    SpriteRenderer(uint8_t quadId, uint8_t materialId, uint8_t textureId, uint8_t atlasIndex, const Color& color)
        : quadId(quadId), materialId(materialId), textureId(textureId), atlasIndex(atlasIndex), color(color) {}
    ~SpriteRenderer();

    const uint8_t quadId = 0;
    const uint8_t textureId = 0;
    const uint8_t atlasIndex = 0;
    const uint8_t materialId = 0;
    const ME::Color color;
};

}  // namespace ME
