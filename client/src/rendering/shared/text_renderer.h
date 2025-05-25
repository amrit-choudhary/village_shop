/**
 * Used to render text in the game.
 * This class handles the rendering of text using a font atlas.
 * It uses a sprite renderer to render the text as sprites.
 * Can render ASCII characters using bitmap font map.
 */

#pragma once

#include <cstdint>

#include "color.h"

namespace ME {

class TextRenderer {
   public:
    TextRenderer() = delete;
    TextRenderer(uint8_t quadId, uint8_t materialId, uint8_t textureId, const Color& color, const char* text)
        : quadId(quadId), materialId(materialId), textureId(textureId), color(color), text(text) {}
    ~TextRenderer();

    const char* text;
    const uint8_t quadId = 0;      // ID of the quad to render
    const uint8_t textureId = 0;   // ID of the texture to use
    const uint8_t materialId = 0;  // ID of the material to use
    const ME::Color color;         // Color of the text

    void SetText(const char* newText);
};

}  // namespace ME
