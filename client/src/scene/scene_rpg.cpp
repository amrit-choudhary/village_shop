#include "scene_rpg.h"

#include <cstddef>

#include "client/src/utils/json_utils.h"
#include "shared/src/misc/game_constants.h"
#include "shared/src/random/random_engine.h"

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
    meshPaths.data = new const char*[Constants::MaxMeshCount];
    quadPaths.data = new const char*[Constants::MaxQuadCount];
    texturePaths.data = new const char*[Constants::MaxTextureCount];
    spriteTexturePaths.data = new const char*[Constants::MaxSpriteTextureCount];
    textureAtlasProperties.data = new ME::TextureAtlasProperties[Constants::MaxTextureAtlasPropertiesCount];
    shaderPaths.data = new const char*[Constants::MaxShaderCount];
    textureSamplers.data = new ME::TextureSampler[Constants::MaxSamplerCount];
    transforms.data = new ME::Transform[Constants::MaxTransformCount];
    transforms.count = 0;
    meshRenderers.data = new ME::MeshRenderer[Constants::MaxMeshRendererCount];
    meshRenderers.count = 0;
    spriteTransforms.data = new ME::Transform[Constants::MaxSpriteTransformCount];
    spriteTransforms.count = 0;
    spriteRenderers.data = new ME::SpriteRenderer[Constants::MaxSpriteRendererCount];
    spriteRenderers.count = 0;
    instancedSpriteTransforms0.data = new ME::Transform[Constants::MaxInstancedSpriteTransformCount];
    instancedSpriteTransforms0.count = 0;
    instancedSpriteRenderers0.data = new ME::SpriteRenderer[Constants::MaxInstancedSpriteRendererCount];
    instancedSpriteRenderers0.count = 0;
    spriteInstanceData0 = new ME::SpriteRendererInstanceData[Constants::MaxInstancedSpriteRendererCount];

    staticColliders.data = new ME::ColliderAABB[Constants::MaxStaticColliderCount];
    staticColliders.count = 0;
    dynamicColliders.data = new ME::ColliderAABB[Constants::MaxDynamicColliderCount];
    dynamicColliders.count = 0;

    meshPaths.count = 0;

    quadPaths[0] = "meshes/quad.obj";
    quadPaths.count = 1;

    texturePaths.count = 0;

    spriteTexturePaths[0] = "textures/sprites/tileset_legacy.png";
    spriteTexturePaths.count = 1;

    shaderPaths[0] = "shaders/metal/sprite.metal";
    shaderPaths[1] = "shaders/metal/sprite_instanced.metal";
    shaderPaths.count = 2;

    ME::JsonUtils::LoadTextureAtlasProps("texture_data/atlas_01.json", textureAtlasProperties[0]);
    textureAtlasProperties.count = 1;

    textureSamplers[0] = ME::TextureSampler(ME::TextureFilter::Nearest, ME::TextureWrap::Repeat);
    textureSamplers.count = 1;
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
    instancedSpriteTransforms0.count = static_cast<uint32_t>(gridCount);

    for (size_t iy = 0; iy < gridHeight; ++iy) {
        for (size_t ix = 0; ix < gridWidth; ++ix) {
            size_t i = (iy * gridWidth) + ix;

            float px = originX + static_cast<float>(cellSizeby2 + (ix * (cellSize + cellPadding)));
            float py = originY + static_cast<float>(cellSizeby2 + (iy * (cellSize + cellPadding)));

            instancedSpriteTransforms0[i].SetPosition(px, py, 1.0f);
            instancedSpriteTransforms0[i].SetScale(cellSize, cellSize);
        }
    }
}

void ME::SceneRPG::BuildInstancedSpriteRenderers() {
    instancedSpriteRenderers0.count = static_cast<uint32_t>(gridCount);

    ME::Random rnd;
    for (size_t i = 0; i < instancedSpriteRenderers0.count; ++i) {
        instancedSpriteRenderers0[i] = ME::SpriteRenderer(0, 0, 2, 1, 1, ME::Color::White());

        spriteInstanceData0[i].modelMatrixData = instancedSpriteTransforms0[i].GetModelMatrix().GetDataForShader();
        spriteInstanceData0[i].atlasIndex = 0;
        spriteInstanceData0[i].color = ME::Color::White();
    }
}
