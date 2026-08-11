#include "client/src/game/evolution/scene_evolution.h"

#include <client/src/utils/json_utils.h>

#include "shared/src/random/random_engine.h"
#include "shared/src/random/stb_perlin.h"

// Local helpers.
static void TerrainGen(ME::Grid<uint8_t>* oceanBase, ME::Grid<uint8_t>* ocean, ME::Grid<uint8_t>* biome,
                       ME::Grid<uint8_t>* terrain, size_t oceanMapSize, size_t biomeMapSize, size_t mapSize);
// End local helpers.

ME::SceneEvolution::SceneEvolution() {}

ME::SceneEvolution::~SceneEvolution() {
    delete oceanBase;
    delete ocean;
    delete biome;
    delete terrain;
}

void ME::SceneEvolution::Init() {
    oceanBase = new ME::Grid<uint8_t>(oceanMapSize, oceanMapSize);
    ocean = new ME::Grid<uint8_t>(oceanMapSize, oceanMapSize);
    biome = new ME::Grid<uint8_t>(biomeMapSize, biomeMapSize);
    terrain = new ME::Grid<uint8_t>(mapSize, mapSize);
    TerrainGen(oceanBase, ocean, biome, terrain, oceanMapSize, biomeMapSize, mapSize);

    Scene::Init();
}

void ME::SceneEvolution::CreateResources() {
    Scene::CreateResources();

    // Slot 1 is unused now. Only there to prevent Renderer crash. Fix later.

    spriteTexturePaths[0] = "textures/world/tileset_evolution.dds";
    spriteTexturePaths[1] = "textures/world/tileset_evolution.dds";
    spriteTextureCount = 2;

    ME::JsonUtils::LoadTextureAtlasProps("texture_data/atlas_tileset_evolution.json", textureAtlasProperties[0]);
    ME::JsonUtils::LoadTextureAtlasProps("texture_data/atlas_tileset_evolution.json", textureAtlasProperties[1]);
    textureAtlasPropertiesCount = 2;
}

void ME::SceneEvolution::BuildLights() {
    Scene::BuildLights();
}

void ME::SceneEvolution::BuildCamera() {
    Scene::BuildCamera();
    spriteCamera->orthographicSize = 100.0f;
}

void ME::SceneEvolution::BuildTransforms() {
    Scene::BuildTransforms();
}

void ME::SceneEvolution::BuildMeshRenderers() {
    Scene::BuildMeshRenderers();
}

void ME::SceneEvolution::BuildSpriteTransforms() {
    Scene::BuildSpriteTransforms();
}

void ME::SceneEvolution::BuildSpriteRenderers() {
    Scene::BuildSpriteRenderers();
}

void ME::SceneEvolution::BuildInstancedSpriteTransforms() {
    Scene::BuildInstancedSpriteTransforms();

    for (size_t i = 0; i < mapSize * mapSize; ++i) {
        float x = static_cast<float>(i % mapSize) * tileSize + originX;
        float y = static_cast<float>(i / mapSize) * tileSize + originY;

        // Adding to instance buffer 0;
        AddInstancedSpriteTransform(ME::Vec3(x, y, 0.0f), ME::Vec3(tileSize, tileSize, 1.0f), 0);
    }
}

void ME::SceneEvolution::BuildInstancedSpriteRenderers() {
    Scene::BuildInstancedSpriteRenderers();

    ME::Random rnd{"tile", true};
    const uint8_t kHeightLevels = 8;
    for (size_t i = 0; i < mapSize * mapSize; ++i) {
        uint8_t height = *(terrain->GetUnsafe(i % mapSize, i / mapSize));
        uint8_t tileIndex;
        ME::Color color = ME::Color::White();
        if (height == 0) {
            tileIndex = 8;  // Ocean tile
        } else if (height == 1 || height == 2) {
            tileIndex = 7;  // Lake
        } else if (height == 3) {
            tileIndex = 6;  // Beach
        } else if (height == 5) {
            tileIndex = 2;  // Forest
        } else if (height > 5) {
            tileIndex = 3;  // Mountain
        } else {
            tileIndex = 0;
        }
        if (height > 0) {
            float shade =
                static_cast<float>(height - 1) / static_cast<float>(kHeightLevels - 1);  // [0,1], black to white
            color = ME::Color(shade, shade, shade, 1.0f);
            color = ME::Color::White();
        }
        ME::SpriteRenderer* spRend = new ME::SpriteRenderer(0, 0, 0, 0, tileIndex, color);
        AddInstancedSpriteRenderer(spRend);
    }
}

const char* ME::SceneEvolution::GetDisplayName() const {
    return "Evolution Scene";
}

static void TerrainGen(ME::Grid<uint8_t>* oceanBase, ME::Grid<uint8_t>* ocean, ME::Grid<uint8_t>* biome,
                       ME::Grid<uint8_t>* terrain, size_t oceanMapSize, size_t biomeMapSize, size_t mapSize) {
RETRY_OCEAN_GEN:
    ME::Random rnd{"oceanBase", true};
    for (size_t i = 0; i < oceanMapSize * oceanMapSize; ++i) {
        double f = rnd.NextDouble();
        if (f < 0.4f) {
            *(oceanBase->GetUnsafe(i % oceanMapSize, i / oceanMapSize)) = 0;  // Set as ocean tile
        } else {
            *(oceanBase->GetUnsafe(i % oceanMapSize, i / oceanMapSize)) = 1;  // Set as land tile
        }
    }

    ocean->CopyFrom(*oceanBase);

    for (size_t y = 0; y < oceanMapSize; y++) {
        for (size_t x = 0; x < oceanMapSize; x++) {
            uint8_t* neighs[8];
            oceanBase->GetNeighbors8(x, y, neighs);

            size_t oceanNeighbors = 0;
            for (size_t n = 0; n < 8; n++) {
                if (neighs[n] != nullptr && *(neighs[n]) == 0) {
                    oceanNeighbors++;
                }
            }

            // If a cell is land and has 4 or more ocean neighbors, it becomes ocean.
            if (*(oceanBase->GetUnsafe(x, y)) == 1 && oceanNeighbors >= 4) {
                *(ocean->GetUnsafe(x, y)) = 0;
            }

            // If a cell is ocean and has 4 or more land neighbors, it becomes land.
            if (*(oceanBase->GetUnsafe(x, y)) == 0 && oceanNeighbors <= 4) {
                *(ocean->GetUnsafe(x, y)) = 1;
            }
        }
    }

    oceanBase->CopyFrom(*ocean);

    for (size_t y = 0; y < oceanMapSize; y++) {
        for (size_t x = 0; x < oceanMapSize; x++) {
            uint8_t* neighs[8];
            oceanBase->GetNeighbors8(x, y, neighs);

            size_t oceanNeighbors = 0;
            for (size_t n = 0; n < 8; n++) {
                if (neighs[n] != nullptr && *(neighs[n]) == 0) {
                    oceanNeighbors++;
                }
            }

            // If a cell is land and has 4 or more ocean neighbors, it becomes ocean.
            if (*(oceanBase->GetUnsafe(x, y)) == 1 && oceanNeighbors >= 4) {
                *(ocean->GetUnsafe(x, y)) = 0;
            }

            // If a cell is ocean and has 4 or more land neighbors, it becomes land.
            if (*(oceanBase->GetUnsafe(x, y)) == 0 && oceanNeighbors <= 4) {
                *(ocean->GetUnsafe(x, y)) = 1;
            }
        }
    }

    // Check if less than 30% ocean tiles. If so, regenerate.
    size_t oceanCount = 0;
    for (size_t i = 0; i < oceanMapSize * oceanMapSize; ++i) {
        if (*(ocean->GetUnsafe(i % oceanMapSize, i / oceanMapSize)) == 0) {
            oceanCount++;
        }
    }

    if (oceanCount < (oceanMapSize * oceanMapSize * 0.3f) || oceanCount > (oceanMapSize * oceanMapSize * 0.5f)) {
        goto RETRY_OCEAN_GEN;
    }

    terrain->FillFrom(*ocean, mapSize / oceanMapSize);

    // Fill height map based on perline noise.
    for (size_t y = 0; y < mapSize; y++) {
        for (size_t x = 0; x < mapSize; x++) {
            if (*(terrain->GetUnsafe(x, y)) == 1) {  // Only for land tiles
                float noiseValue =
                    stb_perlin_noise3(static_cast<float>(x) * 0.1f, static_cast<float>(y) * 0.1f, 0.0f, 0, 0, 0);
                const uint8_t kHeightLevels = 8;
                uint8_t level = static_cast<uint8_t>((noiseValue + 1.0f) * 0.5f * kHeightLevels);
                if (level >= kHeightLevels) {
                    level = kHeightLevels - 1;
                }
                *(terrain->GetUnsafe(x, y)) = static_cast<uint8_t>(level + 1);
            }
        }
    }
}
