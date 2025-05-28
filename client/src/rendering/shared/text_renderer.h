/**
 * Used to render text in the game.
 * This class handles the rendering of text using a font atlas.
 * It uses a sprite renderer to render the text as sprites.
 * Can render ASCII characters using bitmap font map.
 */

#pragma once

#include <cstdint>

#include "color.h"
#include "src/math/vec16.h"

namespace ME {

/**
 * Data to hold the text renderer instance data.
 * Used for instanced rendering.
 */
class TextRendererInstanceData {
   public:
    ME::Vec16 modelMatrixData;
    ME::Color color;
    uint16_t atlasIndex;  // Index of the texture atlas
    uint8_t padding[14];  // Padding to align the structure
};

class TextRenderer {
   public:
    TextRenderer() = delete;
    TextRenderer(const char* text, uint8_t quadId, uint8_t textureId, uint8_t materialId, const ME::Color& color,
                 uint16_t height, uint16_t width, int16_t letterSpacing, int16_t lineGap, uint16_t charsPerLine)
        : text(text),
          quadId(quadId),
          textureId(textureId),
          materialId(materialId),
          color(color),
          height(height),
          width(width),
          letterSpacing(letterSpacing),
          lineGap(lineGap),
          charsPerLine(charsPerLine) {}
    ~TextRenderer();

    const char* text;
    const uint8_t quadId = 0;      // ID of the quad to render
    const uint8_t textureId = 0;   // ID of the texture to use
    const uint8_t materialId = 0;  // ID of the material to use
    const ME::Color color;         // Color of the text
    const uint16_t height;         // Height of the text in pixels
    const uint16_t width;          // Width of the text in pixels
    const int16_t letterSpacing;   // Spacing between letters in pixels
    const int16_t lineGap;         // Gap between lines in pixels
    const uint16_t charsPerLine;   // Number of characters per line

    void SetText(const char* newText);

    // Returns the number of characters in the text.
    uint16_t GetCount() const;

    // Returns the width of the rendered text based on the width and letter spacing.
    uint16_t GetRenderWidth() const;

    // Returns the height of the rendered text.
    uint16_t GetRenderHeight() const;
};

}  // namespace ME
