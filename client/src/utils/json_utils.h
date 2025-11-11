#pragma once

/**
 * JSON Utilities Header.
 * This will load a json file and parse it into a game specific class.
 */

namespace ME {

// Forward Declarations
class TextureAtlasProperties;

class JsonUtils {
   public:
    JsonUtils() = default;
    ~JsonUtils() = default;

    /**
     * Load texture atlas properties from a JSON file.
     * Returns True if the properties were loaded successfully, false otherwise.
     */
    static bool LoadTextureAtlasProps(const char* filePath, ME::TextureAtlasProperties& outAtlasProps);
};

}  // namespace ME
