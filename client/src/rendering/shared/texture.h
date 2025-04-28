/**
 * Texture class for handling textures.
 * This is platform agnostic. This will contain common code for all platforms.
 */

#pragma once

#include <cstdint>

namespace ME {

class Texture {
   public:
    Texture();
    Texture(const char* path);
    ~Texture();

    // Load the texture from a file, using the libpng library.
    void Load(const char* path);

    uint16_t GetWidth() const {
        return width;
    }
    uint16_t GetHeight() const {
        return height;
    }
    uint8_t GetChannels() const {
        return channels;
    }

    bool HasAlpha() const {
        return hasAlpha;
    }

    const uint32_t GetSize() const {
        return width * height * channels;
    }

    const uint8_t* GetData() const {
        return data;
    }

   private:
    uint16_t width;
    uint16_t height;
    uint8_t channels;
    bool hasAlpha;
    uint8_t* data;
};

}  // namespace ME
