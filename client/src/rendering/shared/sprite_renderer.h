/**
 * This is a simple sprite renderer class.
 * It is used to render 2D sprites in the game.
 */

#pragma once

#include <cstdint>

#include "../../anim/sprite_animator.h"
#include "color.h"
#include "src/math/transform.h"
#include "src/math/vec16.h"

namespace ME {

/**
 * Data to hold the sprite renderer instance data.
 * Used for instanced rendering.
 * This structure is sent to the GPU as a constant buffer, so some hlsl padding rules apply.
 * Flags Rules bit wise:
 * 0: Flip the sprite horizontally.
 * 1: Flip the sprite vertically.
 * 2: Flip the sprite diagonally.
 */
class SpriteRendererInstanceData {
   public:
    ME::Vec16 modelMatrixData = ME::Vec16();
    ME::Color color = ME::Color::White();
    uint32_t atlasIndex = 0;  // Index of the texture atlas
    uint32_t flags = 0;       // Additional flags for rendering options
    uint32_t padding[2]{0};   // Padding to align the structure
};

class SpriteRenderer {
   public:
    SpriteRenderer() = delete;
    SpriteRenderer(uint8_t quadId, uint8_t materialId, uint8_t textureId, uint8_t atlasIndex, const Color& color)
        : quadId(quadId), materialId(materialId), textureId(textureId), atlasIndex(atlasIndex), color(color) {}
    ~SpriteRenderer();

    // Mark the sprite renderer as dirty to update rendering data before frame is drawn.
    bool bDirty = true;

    const uint8_t quadId = 0;
    const uint8_t textureId = 0;
    uint16_t atlasIndex = 0;
    const uint8_t materialId = 0;
    ME::Color color;

    ME::SpriteAnimator* animator = nullptr;
};

}  // namespace ME
