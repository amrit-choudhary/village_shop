#include "text_renderer.h"

ME::TextRenderer::~TextRenderer() {}

void ME::TextRenderer::SetText(const char* newText) {
    text = newText;
}
