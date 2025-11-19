#include "scene_dice_simple.h"

#include <cstddef>

#include "../anim/sprite_anim_clip.h"
#include "../anim/sprite_animator.h"
#include "../utils/json_utils.h"
#include "src/misc/game_constants.h"
#include "src/random/random_engine.h"

ME::SceneDiceSimple::SceneDiceSimple() {}
ME::SceneDiceSimple::~SceneDiceSimple() {}

void ME::SceneDiceSimple::Init() {
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

void ME::SceneDiceSimple::CreateResources() {
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
    instancedSpriteTransforms = new ME::Transform*[Constants::MaxInstancedSpriteTransformCount];
    instancedSpriteRenderers = new ME::SpriteRenderer*[Constants::MaxInstancedSpriteRendererCount];
    spriteInstanceData = new ME::SpriteRendererInstanceData*[Constants::MaxInstancedSpriteRendererCount];

    staticColliders = new ME::ColliderAABB[Constants::MaxStaticColliderCount];
    dynamicColliders = new ME::ColliderAABB[Constants::MaxDynamicColliderCount];

    meshCount = 0;

    quadPaths[0] = "meshes/quad.obj";
    quadCount = 1;

    textureCount = 0;

    spriteTexturePaths[0] = "textures/sprites/dice_white.png";
    spriteTexturePaths[1] = "textures/sprites/dice_pink.png";
    spriteTextureCount = 2;

    shaderPaths[0] = "shaders/metal/sprite.metal";
    shaderPaths[1] = "shaders/metal/sprite_instanced.metal";
    shaderCount = 2;

    ME::JsonUtils::LoadTextureAtlasProps("texture_data/atlas_dice.json", textureAtlasProperties[0]);
    ME::JsonUtils::LoadTextureAtlasProps("texture_data/atlas_dice.json", textureAtlasProperties[1]);
    textureAtlasPropertiesCount = 2;

    textureSamplers[0] = ME::TextureSampler(ME::TextureFilter::Nearest, ME::TextureWrap::Repeat);
    textureSamplerCount = 1;
}
void ME::SceneDiceSimple::BuildLights() {
    ME::Scene::BuildLights();
}
void ME::SceneDiceSimple::BuildCamera() {
    ME::Scene::BuildCamera();
    spriteCamera->orthographicSize = 50.0f;
}
void ME::SceneDiceSimple::BuildTransforms() {}
void ME::SceneDiceSimple::BuildMeshRenderers() {}
void ME::SceneDiceSimple::BuildSpriteTransforms() {}
void ME::SceneDiceSimple::BuildSpriteRenderers() {}
void ME::SceneDiceSimple::BuildInstancedSpriteTransforms() {
    // Player Dice
    instancedSpriteTransforms[0] = new ME::Transform();
    instancedSpriteTransforms[0]->SetPosition(0.0f, -diceGap, 1.0f);
    instancedSpriteTransforms[0]->SetScale(cellSize, cellSize);

    // AI Dice
    instancedSpriteTransforms[1] = new ME::Transform();
    instancedSpriteTransforms[1]->SetPosition(0.0f, diceGap, 1.0f);
    instancedSpriteTransforms[1]->SetScale(cellSize, cellSize);

    instancedSpriteTransformCount = 2;
}
void ME::SceneDiceSimple::BuildInstancedSpriteRenderers() {
    instancedSpriteRenderers[0] = new ME::SpriteRenderer(0, 0, 1, 1, 1, ME::Color::White());

    ME::SpriteAnimator* animator0 = new ME::SpriteAnimator(instancedSpriteRenderers[0], 8);
    instancedSpriteRenderers[0]->animator = animator0;

    ME::SpriteAnimClip* clip0 = nullptr;
    ME::JsonUtils::LoadSpriteAnimClipFromJSON("anim/sprite_anim_01.json", &clip0);
    animator0->AddClip(clip0);

    spriteInstanceData[0] = new ME::SpriteRendererInstanceData();
    spriteInstanceData[0]->modelMatrixData = instancedSpriteTransforms[0]->GetModelMatrix().GetDataForShader();
    spriteInstanceData[0]->atlasIndex = 0;
    spriteInstanceData[0]->color = ME::Color::White();

    instancedSpriteRenderers[1] = new ME::SpriteRenderer(0, 0, 2, 1, 1, ME::Color::White());

    ME::SpriteAnimator* animator1 = new ME::SpriteAnimator(instancedSpriteRenderers[1], 8);
    instancedSpriteRenderers[1]->animator = animator1;

    ME::SpriteAnimClip* clip1 = nullptr;
    ME::JsonUtils::LoadSpriteAnimClipFromJSON("anim/sprite_anim_01.json", &clip1);
    animator1->AddClip(clip1);

    spriteInstanceData[1] = new ME::SpriteRendererInstanceData();
    spriteInstanceData[1]->modelMatrixData = instancedSpriteTransforms[1]->GetModelMatrix().GetDataForShader();
    spriteInstanceData[1]->atlasIndex = 0;
    spriteInstanceData[1]->color = ME::Color::White();

    instancedSpriteRendererCount = 2;
}
