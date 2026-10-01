#include "json_utils.h"

#include "shared/src/file_io/vfs.h"

#include <iostream>
#include <map>
#include <string>

#include "client/src/anim/sprite_anim_clip.h"
#include "client/src/game/wave_data.h"
#include "client/src/rendering/shared/texture.h"
#include "shared/third_party/json/cJSON.h"

bool ME::JsonUtils::LoadTextureAtlasProps(const char* filePath, ME::TextureAtlasProperties& outAtlasProps) {
    cJSON* json = LoadJSONFromFile(filePath);
    if (json != nullptr) {
        // Extract properties from the JSON object. This is case insensitive.
        outAtlasProps.tileSizeX = static_cast<uint32_t>(cJSON_GetObjectItem(json, "tileSizeX")->valueint);
        outAtlasProps.tileSizeY = static_cast<uint32_t>(cJSON_GetObjectItem(json, "tileSizeY")->valueint);
        outAtlasProps.padding = static_cast<uint32_t>(cJSON_GetObjectItem(json, "padding")->valueint);
        outAtlasProps.numTextures = static_cast<uint32_t>(cJSON_GetObjectItem(json, "numTextures")->valueint);
        outAtlasProps.numTilesX = static_cast<uint32_t>(cJSON_GetObjectItem(json, "numTilesX")->valueint);
        outAtlasProps.numTilesY = static_cast<uint32_t>(cJSON_GetObjectItem(json, "numTilesY")->valueint);
        outAtlasProps.width = static_cast<uint32_t>(cJSON_GetObjectItem(json, "width")->valueint);
        outAtlasProps.height = static_cast<uint32_t>(cJSON_GetObjectItem(json, "height")->valueint);
        outAtlasProps.paddingType = static_cast<uint32_t>(cJSON_GetObjectItem(json, "paddingType")->valueint);

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
            wave.spawnRate = static_cast<uint32_t>(cJSON_GetObjectItem(waveItem, "spawnRate")->valueint);
        }

        *outWaveData = waveData;

        cJSON_Delete(json);
        return true;
    }

    return false;
}

cJSON* ME::JsonUtils::LoadJSONFromFile(const char* filePath) {
    std::string text;
    if (!Vfs::ReadText(FileRoot::Resources, filePath, text)) {
        std::cout << "Unable to open file: " << Vfs::GetRootPath(FileRoot::Resources) << filePath << std::endl;
        return nullptr;
    }

    cJSON* json = cJSON_Parse(text.c_str());
    if (!json) {
        std::cout << "Failed to parse JSON: " + std::string(cJSON_GetErrorPtr()) << std::endl;
        return nullptr;
    }
    return json;
}
