#include "scene_falling_sand.h"

#include <cstddef>

#include "shared/src/misc/game_constants.h"
#include "shared/src/random/random_engine.h"

ME::SceneFallingSand::SceneFallingSand() {}

ME::SceneFallingSand::~SceneFallingSand() {}

void ME::SceneFallingSand::Init() {
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

void ME::SceneFallingSand::CreateResources() {
    meshPaths.data = new const char*[Constants::MaxMeshCount];
    quadPaths.data = new const char*[Constants::MaxQuadCount];
    texturePaths.data = new const char*[Constants::MaxTextureCount];
    spriteTexturePaths.data = new const char*[Constants::MaxSpriteTextureCount];
    textureAtlasProperties = new ME::TextureAtlasProperties[Constants::MaxTextureAtlasPropertiesCount];
    shaderPaths.data = new const char*[Constants::MaxShaderCount];
    textureSamplers = new ME::TextureSampler[Constants::MaxSamplerCount];
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

    spriteTexturePaths[0] = "textures/sprites/monochrome.png";
    spriteTexturePaths.count = 1;

    shaderPaths[0] = "shaders/metal/sprite.metal";
    shaderPaths[1] = "shaders/metal/sprite_instanced.metal";
    shaderPaths.count = 2;

    textureAtlasProperties[0] = ME::TextureAtlasProperties{17, 17, 1, 1078, 49, 22, 832, 373};
    textureAtlasPropertiesCount = 1;

    textureSamplers[0] = ME::TextureSampler(ME::TextureFilter::Nearest, ME::TextureWrap::Repeat);
    textureSamplerCount = 1;
}

void ME::SceneFallingSand::BuildLights() {
    ME::Scene::BuildLights();
}

void ME::SceneFallingSand::BuildCamera() {
    ME::Scene::BuildCamera();
}

void ME::SceneFallingSand::BuildTransforms() {}

void ME::SceneFallingSand::BuildMeshRenderers() {}

void ME::SceneFallingSand::BuildSpriteTransforms() {}

void ME::SceneFallingSand::BuildSpriteRenderers() {}

void ME::SceneFallingSand::BuildInstancedSpriteTransforms() {
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

void ME::SceneFallingSand::BuildInstancedSpriteRenderers() {
    instancedSpriteRenderers0.count = static_cast<uint32_t>(gridCount);

    for (size_t i = 0; i < instancedSpriteRenderers0.count; ++i) {
        instancedSpriteRenderers0[i] = ME::SpriteRenderer(0, 0, 2, 1, 1, ME::Color::White());

        spriteInstanceData0[i].modelMatrixData = instancedSpriteTransforms0[i].GetModelMatrix().GetDataForShader();
        spriteInstanceData0[i].atlasIndex = 3;
        spriteInstanceData0[i].color = ME::Color::Black();
    }
}
