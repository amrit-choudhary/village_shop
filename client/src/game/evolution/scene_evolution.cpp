#include "client/src/game/evolution/scene_evolution.h"

#include <client/src/utils/json_utils.h>

#include "shared/src/math/math.h"
#include "shared/src/random/perlin_noise.h"
#include "shared/src/random/random_engine.h"

// Local helpers.
static void TerrainGen(ME::Grid<ME::TerrainType>* oceanBase, ME::Grid<ME::TerrainType>* ocean,
                       ME::Grid<ME::TerrainType>* biome, ME::Grid<ME::TerrainType>* terrain, size_t oceanMapSize,
                       size_t biomeMapSize, size_t mapSize);

static void CreatureGen(ME::Creature* creatures, size_t creatureCount, ME::Grid<uint8_t>* walkableMap, size_t mapSize);

// End local helpers.

ME::SceneEvolution::SceneEvolution() {}

ME::SceneEvolution::~SceneEvolution() {
    delete oceanBase;
    delete ocean;
    delete biome;
    delete terrain;
    delete walkableMap;
    delete[] creatures;
}

void ME::SceneEvolution::Init() {
    oceanBase = new ME::Grid<TerrainType>(oceanMapSize, oceanMapSize);
    ocean = new ME::Grid<TerrainType>(oceanMapSize, oceanMapSize);
    biome = new ME::Grid<TerrainType>(biomeMapSize, biomeMapSize);
    terrain = new ME::Grid<TerrainType>(mapSize, mapSize);
    TerrainGen(oceanBase, ocean, biome, terrain, oceanMapSize, biomeMapSize, mapSize);
    walkableMap = new ME::Grid<uint8_t>(mapSize, mapSize);

    for (size_t i = 0; i < mapSize * mapSize; ++i) {
        ME::TerrainType tileType = *(terrain->GetUnsafe(i));
        if (tileType == ME::TerrainType::Ocean || tileType == ME::TerrainType::Lake ||
            tileType == ME::TerrainType::Rocky || tileType == ME::TerrainType::Forest) {
            *(walkableMap->GetUnsafe(i)) = 0;
        } else {
            *(walkableMap->GetUnsafe(i)) = 1;
        }
    }

    creatures = new Creature[creatureCount];
    CreatureGen(creatures, creatureCount, walkableMap, mapSize);

    Scene::Init();
}

void ME::SceneEvolution::CreateResources() {
    Scene::CreateResources();

    // Slot 1 is unused now. Only there to prevent Renderer crash. Fix later.

    spriteTexturePaths[0] = "textures/world/tileset_evolution.dds";
    spriteTexturePaths[1] = "textures/world/tileset_evolution.dds";
    spriteTexturePaths[2] = "textures/world/tileset_evolution.dds";
    spriteTextureCount = 3;

    ME::JsonUtils::LoadTextureAtlasProps("texture_data/atlas_tileset_evolution.json", textureAtlasProperties[0]);
    ME::JsonUtils::LoadTextureAtlasProps("texture_data/atlas_tileset_evolution.json", textureAtlasProperties[1]);
    ME::JsonUtils::LoadTextureAtlasProps("texture_data/atlas_tileset_evolution.json", textureAtlasProperties[2]);
    textureAtlasPropertiesCount = 3;
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

    // Add terrain.
    for (size_t i = 0; i < mapSize * mapSize; ++i) {
        float x = static_cast<float>(i % mapSize) * tileSize + originX;
        float y = static_cast<float>(i / mapSize) * tileSize + originY;

        // Adding to instance buffer 0; pushed behind buffer 1 (creatures) in depth so they draw on top.
        AddInstancedSpriteTransform(ME::Vec3(x, y, 1.0f), ME::Vec3(tileSize, tileSize, 1.0f), 0);
    }

    // Add creatures.
    for (size_t i = 0; i < creatureCount; ++i) {
        float x = static_cast<float>(creatures[i].position.x) * tileSize + originX;
        float y = static_cast<float>(creatures[i].position.y) * tileSize + originY;

        AddInstancedSpriteTransform(ME::Vec3(x, y, 0.0f), ME::Vec3(tileSize, tileSize, 1.0f), 1);
    }
}

void ME::SceneEvolution::BuildInstancedSpriteRenderers() {
    Scene::BuildInstancedSpriteRenderers();

    // Add terrain.
    for (size_t i = 0; i < mapSize * mapSize; ++i) {
        uint8_t tileIndex = static_cast<uint8_t>(*(terrain->GetUnsafe(i)));
        ME::SpriteRenderer* spRend = new ME::SpriteRenderer(0, 0, 0, 0, tileIndex, ME::Color::White(), 0);
        AddInstancedSpriteRenderer(spRend, 0);
    }

    // Add creatures.
    for (size_t i = 0; i < creatureCount; ++i) {
        ME::SpriteRenderer* spRend = new ME::SpriteRenderer(0, 0, 0, 0, 10, ME::Color::White(), 0);
        AddInstancedSpriteRenderer(spRend, 1);
    }
}

const char* ME::SceneEvolution::GetDisplayName() const {
    return "Evolution Scene";
}

void ME::SceneEvolution::UpdateCreatures(float deltaTime) {
    for (size_t i = 0; i < creatureCount; ++i) {
        ME::Vec2 prevPosition = creatures[i].position;
        creatures[i].Update(deltaTime, walkableMap, creatureRnd);

        if (creatures[i].position != prevPosition) {
            float x = creatures[i].position.x * tileSize + originX;
            float y = creatures[i].position.y * tileSize + originY;
            instancedSpriteTransforms1[i]->SetPosition(x, y, 0.0f);
            instancedSpriteRenderers1[i]->bDirty = true;
        }
    }
}

static void TerrainGen(ME::Grid<ME::TerrainType>* oceanBase, ME::Grid<ME::TerrainType>* ocean,
                       ME::Grid<ME::TerrainType>* biome, ME::Grid<ME::TerrainType>* terrain, size_t oceanMapSize,
                       size_t biomeMapSize, size_t mapSize) {
    biome->Fill(ME::TerrainType::Grass);  // For now, biome is not used. Fill with Grass.
    biomeMapSize = 0;                     // Not used for now.

RETRY_OCEAN_GEN:
    ME::Random rnd{"oceanBase", true};
    for (size_t i = 0; i < oceanMapSize * oceanMapSize; ++i) {
        double f = rnd.NextDouble();
        if (f < 0.3f) {
            *(oceanBase->GetUnsafe(i)) = ME::TerrainType::Ocean;
        } else {
            *(oceanBase->GetUnsafe(i)) = ME::TerrainType::Grass;
        }
    }

    ocean->CopyFrom(*oceanBase);

    for (size_t y = 0; y < oceanMapSize; y++) {
        for (size_t x = 0; x < oceanMapSize; x++) {
            ME::TerrainType* neighs[8];
            oceanBase->GetNeighbors8(x, y, neighs);

            size_t oceanNeighbors = 0;
            for (size_t n = 0; n < 8; n++) {
                if (neighs[n] != nullptr && *(neighs[n]) == ME::TerrainType::Ocean) {
                    oceanNeighbors++;
                }
            }

            // If a cell is land and has 4 or more ocean neighbors, it becomes ocean.
            if (*(oceanBase->GetUnsafe(x, y)) == ME::TerrainType::Grass && oceanNeighbors >= 4) {
                *(ocean->GetUnsafe(x, y)) = ME::TerrainType::Ocean;
            }

            // If a cell is ocean and has 4 or more land neighbors, it becomes land.
            if (*(oceanBase->GetUnsafe(x, y)) == ME::TerrainType::Ocean && oceanNeighbors <= 4) {
                *(ocean->GetUnsafe(x, y)) = ME::TerrainType::Grass;
            }
        }
    }

    oceanBase->CopyFrom(*ocean);

    for (size_t y = 0; y < oceanMapSize; y++) {
        for (size_t x = 0; x < oceanMapSize; x++) {
            ME::TerrainType* neighs[8];
            oceanBase->GetNeighbors8(x, y, neighs);

            size_t oceanNeighbors = 0;
            for (size_t n = 0; n < 8; n++) {
                if (neighs[n] != nullptr && *(neighs[n]) == ME::TerrainType::Ocean) {
                    oceanNeighbors++;
                }
            }

            // If a cell is land and has 4 or more ocean neighbors, it becomes ocean.
            if (*(oceanBase->GetUnsafe(x, y)) == ME::TerrainType::Grass && oceanNeighbors >= 4) {
                *(ocean->GetUnsafe(x, y)) = ME::TerrainType::Ocean;
            }

            // If a cell is ocean and has 4 or more land neighbors, it becomes land.
            if (*(oceanBase->GetUnsafe(x, y)) == ME::TerrainType::Ocean && oceanNeighbors <= 4) {
                *(ocean->GetUnsafe(x, y)) = ME::TerrainType::Grass;
            }
        }
    }

    // Check if less than 30% ocean tiles. If so, regenerate.
    size_t oceanCount = 0;
    for (size_t i = 0; i < oceanMapSize * oceanMapSize; ++i) {
        if (*(ocean->GetUnsafe(i)) == ME::TerrainType::Ocean) {
            oceanCount++;
        }
    }

    if (oceanCount < (oceanMapSize * oceanMapSize * 0.2f) || oceanCount > (oceanMapSize * oceanMapSize * 0.4f)) {
        goto RETRY_OCEAN_GEN;
    }

    terrain->FillFrom(*ocean, mapSize / oceanMapSize);

    // Fill height map based on fractal perlin noise.
    ME::Random rnd2{"perlin", true};
    ME::PerlinNoise noise{rnd2};
    // lacunarity=2.0, gain=0.5, octaves=6 -- typical fbm defaults (see stb_perlin.h).
    const float kFbmLacunarity = 2.0f;
    const float kFbmGain = 0.5f;
    const int kFbmOctaves = 6;
    // Octaves partially cancel each other out, so fbm's output almost never approaches its
    // analytic max amplitude -- normalizing against that max leaves values clustered near 0.
    // kFbmContrast pulls the typically-observed spread back out to fill [-1, 1]; retune by eye
    // if the level distribution still looks off.
    const float kFbmContrast = 3.5f;
    const float kFbmMaxAmplitude = ME::PerlinNoise::FbmMaxAmplitude(kFbmGain, kFbmOctaves);
    for (size_t y = 0; y < mapSize; y++) {
        for (size_t x = 0; x < mapSize; x++) {
            if (*(terrain->GetUnsafe(x, y)) == ME::TerrainType::Grass) {  // Only for land tiles
                float noiseValue = noise.Fbm(static_cast<float>(x) * 0.1f, static_cast<float>(y) * 0.1f, 0.0f,
                                             kFbmLacunarity, kFbmGain, kFbmOctaves) /
                                   kFbmMaxAmplitude * kFbmContrast;
                noiseValue = ME::Math::Clamp(noiseValue, -1.0f, 1.0f);
                const uint8_t kHeightLevels = 16;
                uint8_t level = static_cast<uint8_t>((noiseValue + 1.0f) * 0.5f * kHeightLevels);
                if (level >= kHeightLevels) {
                    level = kHeightLevels - 1;
                }

                uint8_t terrainIndex = 0;
                switch (level) {
                    case 0:
                        terrainIndex = 7;
                        break;
                    case 1:
                        terrainIndex = 7;
                        break;
                    case 2:
                        terrainIndex = 7;
                        break;
                    case 3:
                        terrainIndex = 6;
                        break;
                    case 4:
                        terrainIndex = 6;
                        break;
                    case 5:
                        terrainIndex = 6;
                        break;
                    case 6:
                        terrainIndex = 0;
                        break;
                    case 7:
                        terrainIndex = 0;
                        break;
                    case 8:
                        terrainIndex = 0;
                        break;
                    case 9:
                        terrainIndex = 0;
                        break;
                    case 10:
                        terrainIndex = 0;
                        break;
                    case 11:
                        terrainIndex = 0;
                        break;
                    case 12:
                        terrainIndex = 1;
                        break;
                    case 13:
                        terrainIndex = 1;
                        break;
                    case 14:
                        terrainIndex = 2;
                        break;
                    case 15:
                        terrainIndex = 3;
                        break;
                    case 16:
                        terrainIndex = 3;
                        break;
                    default:
                        terrainIndex = 0;
                        break;
                }

                *(terrain->GetUnsafe(x, y)) = static_cast<ME::TerrainType>(terrainIndex);
            }
        }
    }
}

static void CreatureGen(ME::Creature* creatures, size_t creatureCount, ME::Grid<uint8_t>* walkableMap, size_t mapSize) {
    ME::Random rnd{"creatureGen", true};

    for (size_t i = 0; i < creatureCount; ++i) {
        ME::Creature* creature = &creatures[i];

        // Randomly place creature on walkable tile.
        while (true) {
            size_t x = rnd.NextRange(0, mapSize - 1);
            size_t y = rnd.NextRange(0, mapSize - 1);
            if (*(walkableMap->GetUnsafe(x, y)) == 1) {
                creature->position = ME::Vec2(static_cast<float>(x), static_cast<float>(y));
                break;
            }
        }
    }
}
