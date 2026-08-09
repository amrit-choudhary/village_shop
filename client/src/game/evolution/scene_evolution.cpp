#include "client/src/game/evolution/scene_evolution.h"

#include <client/src/utils/json_utils.h>

#include "shared/src/random/random_engine.h"

ME::SceneEvolution::SceneEvolution() {}

ME::SceneEvolution::~SceneEvolution() {}

void ME::SceneEvolution::Init() {
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

    for (size_t i = 0; i < 100 * 100; ++i) {
        float x = static_cast<float>(i % 100) * tileSize + originX;
        float y = static_cast<float>(i / 100) * tileSize + originY;

        // Adding to instance buffer 0;
        AddInstancedSpriteTransform(ME::Vec3(x, y, 0.0f), ME::Vec3(tileSize, tileSize, 1.0f), 0);
    }
}

void ME::SceneEvolution::BuildInstancedSpriteRenderers() {
    Scene::BuildInstancedSpriteRenderers();

    ME::Random rnd{"tile", true};
    for (size_t i = 0; i < 100 * 100; ++i) {
        uint8_t tileIndex =
            static_cast<uint8_t>(rnd.NextRange(0, 8));  // Assuming there are 10 tiles in the atlas (0-9)

        ME::SpriteRenderer* spRend = new ME::SpriteRenderer(0, 0, 0, 0, tileIndex, ME::Color::White());
        AddInstancedSpriteRenderer(spRend);
    }
}

const char* ME::SceneEvolution::GetDisplayName() const {
    return "Evolution Scene";
}
