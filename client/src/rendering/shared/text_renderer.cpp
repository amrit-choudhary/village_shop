#include "text_renderer.h"

#include <cstring>

ME::TextRenderer::~TextRenderer() {}

void ME::TextRenderer::SetText(const char* newText) {
    text = newText;
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
