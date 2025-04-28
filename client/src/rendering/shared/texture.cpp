#include "texture.h"

#include <iostream>

#include "image_loader_png.h"
#include "src/logging.h"

ME::Texture::Texture() {}

ME::Texture::Texture(const char* path) {
    Load(path);
}

ME::Texture::~Texture() {
    delete[] data;
}

void ME::Texture::Load(const char* path) {
    PNGData pngData;
    if (!LoadPNG(path, pngData)) {
        ME::Log("Failed to load PNG file: ");
        return;
    }

    width = pngData.width;
    height = pngData.height;
    channels = pngData.channels;
    hasAlpha = pngData.has_alpha;

    data = new uint8_t[GetSize()];
    std::memcpy(data, pngData.pixels.data(), GetSize());
}
