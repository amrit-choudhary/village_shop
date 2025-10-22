/**
 * Texture class for handling textures.
 * This is platform agnostic. This will contain common code for all platforms.
 */

#pragma once

#include <cstdint>

namespace ME {

// Constants.
constexpr uint8_t MIP_LEVELS_DEFAULT = 4;

/**
 * Data related to texture atlases.
 * This is used to hold information about the texture atlas, such as the size, number of textures, etc.
 */
class TextureAtlasProperties {
   public:
    uint16_t tileSizeX;    // Width of each tile in the atlas
    uint16_t tileSizeY;    // Size of each tile in the atlas
    uint16_t padding;      // Padding between tiles in the atlas
    uint16_t numTextures;  // Number of textures in the atlas
    uint16_t numTilesX;    // Number of tiles in the X direction
    uint16_t numTilesY;    // Number of tiles in the Y direction
    uint16_t width;        // Width of the atlas texture
    uint16_t height;       // Height of the atlas texture
};

class Texture {
   public:
    Texture();
    Texture(const char* path);
    virtual ~Texture();

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

   protected:
    uint16_t width;
    uint16_t height;
    uint8_t channels;
    bool hasAlpha;
    uint8_t* data;
};

}  // namespace ME
