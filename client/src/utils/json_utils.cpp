#include "json_utils.h"

#include <src/misc/utils.h>

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

#include "../anim/sprite_anim_clip.h"
#include "../game/wave_data.h"
#include "../rendering/shared/texture.h"
#include "src/third_party/json/cJSON.h"

bool ME::JsonUtils::LoadTextureAtlasProps(const char* filePath, ME::TextureAtlasProperties& outAtlasProps) {
    cJSON* json = LoadJSONFromFile(filePath);
    if (json != nullptr) {
        // Extract properties from the JSON object. This is case insensitive.
        outAtlasProps.tileSizeX = cJSON_GetObjectItem(json, "tileSizeX")->valueint;
        outAtlasProps.tileSizeY = cJSON_GetObjectItem(json, "tileSizeY")->valueint;
        outAtlasProps.padding = cJSON_GetObjectItem(json, "padding")->valueint;
        outAtlasProps.numTextures = cJSON_GetObjectItem(json, "numTextures")->valueint;
        outAtlasProps.numTilesX = cJSON_GetObjectItem(json, "numTilesX")->valueint;
        outAtlasProps.numTilesY = cJSON_GetObjectItem(json, "numTilesY")->valueint;
        outAtlasProps.width = cJSON_GetObjectItem(json, "width")->valueint;
        outAtlasProps.height = cJSON_GetObjectItem(json, "height")->valueint;
        outAtlasProps.paddingType = cJSON_GetObjectItem(json, "paddingType")->valueint;

        cJSON_Delete(json);
        return true;
    }
    return false;
}

bool ME::JsonUtils::LoadSpriteAnimClipFromJSON(const char* filePath, ME::SpriteAnimClip** outSpriteAnimClip) {
    *outSpriteAnimClip = nullptr;

    cJSON* json = LoadJSONFromFile(filePath);
    if (json != nullptr) {
        cJSON* bLoopingItem = cJSON_GetObjectItem(json, "bLooping");
        bool bLooping = (bLoopingItem != nullptr) && cJSON_IsTrue(bLoopingItem);
        uint8_t textureAtlasIndex = static_cast<uint8_t>(cJSON_GetObjectItem(json, "textureAtlasIndex")->valueint);
        uint8_t spriteCount = static_cast<uint8_t>(cJSON_GetObjectItem(json, "spriteCount")->valueint);
        uint16_t* spriteIndices = new uint16_t[spriteCount]{};
        cJSON* arr = cJSON_GetObjectItem(json, "spriteIndices");
        for (uint8_t i = 0; i < spriteCount; i++) {
            cJSON* item = cJSON_GetArrayItem(arr, i);
            spriteIndices[i] = static_cast<uint16_t>(item->valueint);
        }

        ME::SpriteAnimClip* clip = new SpriteAnimClip(bLooping, textureAtlasIndex, spriteCount, spriteIndices);
        *outSpriteAnimClip = clip;

        cJSON_Delete(json);
        return true;
    }
    return false;
}

bool ME::JsonUtils::LoadWaveDataFromJSON(const char* filePath, WaveData** outWaveData) {
    *outWaveData = nullptr;

    cJSON* json = LoadJSONFromFile(filePath);
    if (json != nullptr) {
        uint32_t waveCount = static_cast<uint32_t>(cJSON_GetObjectItem(json, "waveCount")->valueint);
        ME::WaveData* waveData = new ME::WaveData();
        waveData->waveCount = waveCount;
        waveData->waves = new ME::SingleWave[waveCount]{};

        cJSON* wavesArray = cJSON_GetObjectItem(json, "waves");
        for (uint32_t i = 0; i < waveCount; i++) {
            cJSON* waveItem = cJSON_GetArrayItem(wavesArray, i);
            ME::SingleWave& wave = waveData->waves[i];
            wave.enemyType = static_cast<uint32_t>(cJSON_GetObjectItem(waveItem, "enemyType")->valueint);
            wave.enemyCount = static_cast<uint32_t>(cJSON_GetObjectItem(waveItem, "enemyCount")->valueint);
            wave.health = static_cast<uint32_t>(cJSON_GetObjectItem(waveItem, "health")->valueint);
            wave.spriteIndex = static_cast<uint32_t>(cJSON_GetObjectItem(waveItem, "spriteIndex")->valueint);
            wave.speedMult = static_cast<uint32_t>(cJSON_GetObjectItem(waveItem, "speedMult")->valueint);
        }

        *outWaveData = waveData;

        cJSON_Delete(json);
        return true;
    }

    return false;
}

cJSON* ME::JsonUtils::LoadJSONFromFile(const char* filePath) {
    std::string fileName = ME::Utils::GetResourcesPath() + filePath;

    std::ifstream file(fileName);
    std::stringstream buffer;

    if (file.is_open()) {
        buffer << file.rdbuf();
        const std::string fileContent = buffer.str();

        cJSON* json = cJSON_Parse(fileContent.c_str());
        if (!json) {
            std::cout << "Failed to parse JSON: " + std::string(cJSON_GetErrorPtr()) << std::endl;
            return nullptr;
        }
        return json;
    } else {
        std::cout << "Unable to open file" << std::endl;
        return nullptr;
    }
    return nullptr;
}
