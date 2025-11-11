#include "json_utils.h"

#include <src/misc/utils.h>

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

#include "../rendering/shared/texture.h"
#include "src/third_party/json/cJSON.h"

bool ME::JsonUtils::LoadTextureAtlasProps(const char* filePath, ME::TextureAtlasProperties& outAtlasProps) {
    std::string fileName = ME::GetResourcesPath() + filePath;

    std::ifstream file(fileName);
    std::stringstream buffer;

    if (file.is_open()) {
        buffer << file.rdbuf();
        const std::string fileContent = buffer.str();

        // Parse the JSON content.
        cJSON* json = cJSON_Parse(fileContent.c_str());
        if (!json) {
            std::cout << "Failed to parse JSON: " + std::string(cJSON_GetErrorPtr()) << std::endl;
            return false;
        }

        // Extract properties from the JSON object. This is case insensitive.
        outAtlasProps.tileSizeX = cJSON_GetObjectItem(json, "tileSizeX")->valueint;
        outAtlasProps.tileSizeY = cJSON_GetObjectItem(json, "tileSizeY")->valueint;
        outAtlasProps.padding = cJSON_GetObjectItem(json, "padding")->valueint;
        outAtlasProps.numTextures = cJSON_GetObjectItem(json, "numTextures")->valueint;
        outAtlasProps.numTilesX = cJSON_GetObjectItem(json, "numTilesX")->valueint;
        outAtlasProps.numTilesY = cJSON_GetObjectItem(json, "numTilesY")->valueint;
        outAtlasProps.width = cJSON_GetObjectItem(json, "width")->valueint;
        outAtlasProps.height = cJSON_GetObjectItem(json, "height")->valueint;

        // Clean up and return the properties.
        cJSON_Delete(json);
        return true;
    } else {
        std::cout << "Unable to open file" << std::endl;
        return false;
    }

    return false;
}
