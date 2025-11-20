#include "scene_rpg.h"

#include <cstddef>

#include "../utils/json_utils.h"
#include "src/misc/game_constants.h"
#include "src/random/random_engine.h"

ME::SceneRPG::SceneRPG() {}

ME::SceneRPG::~SceneRPG() {}

void ME::SceneRPG::Init() {
    CreateResources();
    BuildLights();
    BuildCamera();
    BuildTransforms();
    BuildMeshRenderers();
    BuildSpriteTransforms();
    BuildSpriteRenderers();
    BuildInstancedSpriteTransforms();
    BuildInstancedSpriteRenderers();
}

void ME::SceneRPG::CreateResources() {
    meshPaths = new const char*[Constants::MaxMeshCount];
    quadPaths = new const char*[Constants::MaxQuadCount];
    texturePaths = new const char*[Constants::MaxTextureCount];
    spriteTexturePaths = new const char*[Constants::MaxSpriteTextureCount];
    textureAtlasProperties = new ME::TextureAtlasProperties[Constants::MaxTextureAtlasPropertiesCount];
    shaderPaths = new const char*[Constants::MaxShaderCount];
    textureSamplers = new ME::TextureSampler[Constants::MaxSamplerCount];
    transforms = new ME::Transform*[Constants::MaxTransformCount];
    meshRenderers = new ME::MeshRenderer*[Constants::MaxMeshRendererCount];
    spriteTransforms = new ME::Transform*[Constants::MaxSpriteTransformCount];
    spriteRenderers = new ME::SpriteRenderer*[Constants::MaxSpriteRendererCount];
    instancedSpriteTransforms0 = new ME::Transform*[Constants::MaxInstancedSpriteTransformCount];
    instancedSpriteRenderers0 = new ME::SpriteRenderer*[Constants::MaxInstancedSpriteRendererCount];
    spriteInstanceData0 = new ME::SpriteRendererInstanceData[Constants::MaxInstancedSpriteRendererCount];

    staticColliders = new ME::ColliderAABB[Constants::MaxStaticColliderCount];
    dynamicColliders = new ME::ColliderAABB[Constants::MaxDynamicColliderCount];

    meshCount = 0;

    quadPaths[0] = "meshes/quad.obj";
    quadCount = 1;

    textureCount = 0;

    spriteTexturePaths[0] = "textures/sprites/tileset_legacy.png";
    spriteTextureCount = 1;

    shaderPaths[0] = "shaders/metal/sprite.metal";
    shaderPaths[1] = "shaders/metal/sprite_instanced.metal";
    shaderCount = 2;

    ME::JsonUtils::LoadTextureAtlasProps("texture_data/atlas_01.json", textureAtlasProperties[0]);
    textureAtlasPropertiesCount = 1;

    textureSamplers[0] = ME::TextureSampler(ME::TextureFilter::Nearest, ME::TextureWrap::Repeat);
    textureSamplerCount = 1;
}

void ME::SceneRPG::BuildLights() {
    ME::Scene::BuildLights();
}

void ME::SceneRPG::BuildCamera() {
    ME::Scene::BuildCamera();
    spriteCamera->orthographicSize = 300.0f;
}

void ME::SceneRPG::BuildTransforms() {}

void ME::SceneRPG::BuildMeshRenderers() {}

void ME::SceneRPG::BuildSpriteTransforms() {}

void ME::SceneRPG::BuildSpriteRenderers() {}

void ME::SceneRPG::BuildInstancedSpriteTransforms() {
    instancedSpriteTransformCount0 = static_cast<uint32_t>(gridCount);

    for (size_t iy = 0; iy < gridHeight; ++iy) {
        for (size_t ix = 0; ix < gridWidth; ++ix) {
            size_t i = (iy * gridWidth) + ix;

            float px = originX + static_cast<float>(cellSizeby2 + (ix * (cellSize + cellPadding)));
            float py = originY + static_cast<float>(cellSizeby2 + (iy * (cellSize + cellPadding)));

            instancedSpriteTransforms0[i] = new ME::Transform();
            instancedSpriteTransforms0[i]->SetPosition(px, py, 1.0f);
            instancedSpriteTransforms0[i]->SetScale(cellSize, cellSize);
        }
    }
}

void ME::SceneRPG::BuildInstancedSpriteRenderers() {
    instancedSpriteRendererCount0 = static_cast<uint32_t>(gridCount);

    ME::Random rnd;
    for (size_t i = 0; i < instancedSpriteRendererCount0; ++i) {
        instancedSpriteRenderers0[i] = new ME::SpriteRenderer(0, 0, 2, 1, 1, ME::Color::White());

        spriteInstanceData0[i].modelMatrixData = instancedSpriteTransforms0[i]->GetModelMatrix().GetDataForShader();
        spriteInstanceData0[i].atlasIndex = 0;
        spriteInstanceData0[i].color = ME::Color::White();
    }
}
