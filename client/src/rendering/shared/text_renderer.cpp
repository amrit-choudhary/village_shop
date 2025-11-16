#include "text_renderer.h"

#include <cstring>

ME::TextRenderer::TextRenderer(const char* text, uint8_t quadId, uint8_t textureId, uint8_t materialId,
                               const ME::Color& color, uint16_t height, uint16_t width, int16_t letterSpacing,
                               int16_t lineGap, uint16_t charsPerLine)
    : quadId(quadId), textureId(textureId), materialId(materialId) {
    size_t length = strlen(text) + 1;
    this->text = new char[length];
    strcpy(this->text, text);

    this->color = color;
    this->height = height;
    this->width = width;
    this->letterSpacing = letterSpacing;
    this->lineGap = lineGap;
    this->charsPerLine = charsPerLine;
}

ME::TextRenderer::~TextRenderer() {
    delete[] text;
    text = nullptr;
}

void ME::TextRenderer::SetText(const char* newText) {
    delete[] text;
    text = nullptr;

    size_t length = strlen(newText) + 1;
    text = new char[length];
    strcpy(text, newText);

    bDirty = true;
}

uint16_t ME::TextRenderer::GetCount() const {
    return strlen(text);
}

uint16_t ME::TextRenderer::GetRenderWidth() const {
    return width * GetCount() + letterSpacing * (GetCount() - 1);
}

uint16_t ME::TextRenderer::GetRenderHeight() const {
    return height;
}
