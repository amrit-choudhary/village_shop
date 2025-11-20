#include "scene_game_of_life.h"

#include <cstddef>

#include "src/misc/game_constants.h"
#include "src/random/random_engine.h"

ME::SceneGameOfLife::SceneGameOfLife() {}

ME::SceneGameOfLife::~SceneGameOfLife() {}

void ME::SceneGameOfLife::Init() {
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

void ME::SceneGameOfLife::CreateResources() {
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

    spriteTexturePaths[0] = "textures/sprites/monochrome.png";
    spriteTextureCount = 1;

    shaderPaths[0] = "shaders/metal/sprite.metal";
    shaderPaths[1] = "shaders/metal/sprite_instanced.metal";
    shaderCount = 2;

    textureAtlasProperties[0] = ME::TextureAtlasProperties{17, 17, 1, 1078, 49, 22, 832, 373};
    textureAtlasPropertiesCount = 1;

    textureSamplers[0] = ME::TextureSampler(ME::TextureFilter::Nearest, ME::TextureWrap::Repeat);
    textureSamplerCount = 1;
}

void ME::SceneGameOfLife::BuildLights() {
    ME::Scene::BuildLights();
}

void ME::SceneGameOfLife::BuildCamera() {
    ME::Scene::BuildCamera();
}

void ME::SceneGameOfLife::BuildTransforms() {}

void ME::SceneGameOfLife::BuildMeshRenderers() {}

void ME::SceneGameOfLife::BuildSpriteTransforms() {}

void ME::SceneGameOfLife::BuildSpriteRenderers() {}

void ME::SceneGameOfLife::BuildInstancedSpriteTransforms() {
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

void ME::SceneGameOfLife::BuildInstancedSpriteRenderers() {
    instancedSpriteRendererCount0 = static_cast<uint32_t>(gridCount);

    for (size_t i = 0; i < instancedSpriteRendererCount0; ++i) {
        instancedSpriteRenderers0[i] = new ME::SpriteRenderer(0, 0, 2, 1, 1, ME::Color::White());

        spriteInstanceData0[i].modelMatrixData = instancedSpriteTransforms0[i]->GetModelMatrix().GetDataForShader();
        spriteInstanceData0[i].atlasIndex = 253;
        spriteInstanceData0[i].color = ME::Color::Black();
    }
}
