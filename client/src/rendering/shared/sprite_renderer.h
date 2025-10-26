/**
 * This is a simple sprite renderer class.
 * It is used to render 2D sprites in the game.
 */

#pragma once

#include <cstdint>

#include "color.h"
#include "src/math/transform.h"
#include "src/math/vec16.h"

namespace ME {

/**
 * Data to hold the sprite renderer instance data.
 * Used for instanced rendering.
 * This structure is sent to the GPU as a constant buffer, so some hlsl padding rules apply.
 */
class SpriteRendererInstanceData {
   public:
    ME::Vec16 modelMatrixData;
    ME::Color color;
    uint32_t atlasIndex;  // Index of the texture atlas
    uint32_t padding[3];  // Padding to align the structure
};

class SpriteRenderer {
   public:
    SpriteRenderer() = delete;
    SpriteRenderer(uint8_t quadId, uint8_t materialId, uint8_t textureId, uint8_t atlasIndex, const Color& color)
        : quadId(quadId), materialId(materialId), textureId(textureId), atlasIndex(atlasIndex), color(color) {}
    ~SpriteRenderer();

    const uint8_t quadId = 0;
    const uint8_t textureId = 0;
    const uint16_t atlasIndex = 0;
    const uint8_t materialId = 0;
    const ME::Color color;
};

}  // namespace ME
