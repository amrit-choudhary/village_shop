#include "scene_character_test.h"

#include <cstddef>

#include "../anim/sprite_anim_clip.h"
#include "../anim/sprite_animator.h"
#include "../utils/json_utils.h"
#include "src/misc/game_constants.h"
#include "src/random/random_engine.h"

ME::SceneCharacterTest::SceneCharacterTest() {}

ME::SceneCharacterTest::~SceneCharacterTest() {}

void ME::SceneCharacterTest::Init() {
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

void ME::SceneCharacterTest::CreateResources() {
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

    spriteTexturePaths[0] = "textures/characters/character_walk_8.png";
    spriteTexturePaths[1] = "textures/enemies/enemy_atlas.png";
    spriteTextureCount = 2;

    shaderPaths[0] = "shaders/metal/sprite.metal";
    shaderPaths[1] = "shaders/metal/sprite_instanced.metal";
    shaderCount = 2;

    ME::JsonUtils::LoadTextureAtlasProps("texture_data/atlas_char_8.json", textureAtlasProperties[0]);
    ME::JsonUtils::LoadTextureAtlasProps("texture_data/atlas_enemy.json", textureAtlasProperties[1]);
    textureAtlasPropertiesCount = 2;

    textureSamplers[0] = ME::TextureSampler(ME::TextureFilter::Nearest, ME::TextureWrap::Repeat);
    textureSamplerCount = 1;
}

void ME::SceneCharacterTest::BuildLights() {
    ME::Scene::BuildLights();
}

void ME::SceneCharacterTest::BuildCamera() {
    ME::Scene::BuildCamera();
    spriteCamera->orthographicSize = 50.0f;
}

void ME::SceneCharacterTest::BuildTransforms() {}

void ME::SceneCharacterTest::BuildMeshRenderers() {}

void ME::SceneCharacterTest::BuildSpriteTransforms() {
    // Character Sprite
    spriteTransforms[0] = new ME::Transform();
    spriteTransforms[0]->SetPosition(0.0f, 0.0f, 0.0f);
    spriteTransforms[0]->SetScale(charWidth, charHeight);

    spriteTransformCount = 1;
}

void ME::SceneCharacterTest::BuildSpriteRenderers() {
    spriteRenderers[0] = new ME::SpriteRenderer(0, 0, 0, 0, 1, ME::Color::White());

    ME::SpriteAnimator* animator0 = new ME::SpriteAnimator(spriteRenderers[0], 8);
    spriteRenderers[0]->animator = animator0;

    ME::SpriteAnimClip* clip0 = nullptr;
    ME::JsonUtils::LoadSpriteAnimClipFromJSON("anim/char_anim_up.json", &clip0);

    ME::SpriteAnimClip* clip1 = nullptr;
    ME::JsonUtils::LoadSpriteAnimClipFromJSON("anim/char_anim_right.json", &clip1);

    ME::SpriteAnimClip* clip2 = nullptr;
    ME::JsonUtils::LoadSpriteAnimClipFromJSON("anim/char_anim_down.json", &clip2);

    ME::SpriteAnimClip* clip3 = nullptr;
    ME::JsonUtils::LoadSpriteAnimClipFromJSON("anim/char_anim_left.json", &clip3);

    animator0->AddClip(clip0);
    animator0->AddClip(clip1);
    animator0->AddClip(clip2);
    animator0->AddClip(clip3);

    spriteRendererCount = 1;
}

void ME::SceneCharacterTest::BuildInstancedSpriteTransforms() {
    // NPC Sprites
    ME::Random rnd{"npc_position", true};
    for (size_t i = 0; i < maxNPCCount; ++i) {
        float x = rnd.NextDouble() * 70.0f - 35.0f;
        float y = rnd.NextDouble() * 80.0f - 40.0f;

        instancedSpriteTransforms[i] = new ME::Transform();
        instancedSpriteTransforms[i]->SetPosition(x, y, 0.0f);
        instancedSpriteTransforms[i]->SetScale(npcWidth, npcHeight);
    }
    instancedSpriteTransformCount = maxNPCCount;
}

void ME::SceneCharacterTest::BuildInstancedSpriteRenderers() {
    ME::Random rnd{"npc_type", true};
    ME::SpriteAnimClip* clipBase = nullptr;  // Used as base to duplicate with offset. Deleted afterwards.
    ME::JsonUtils::LoadSpriteAnimClipFromJSON("anim/enemy_base_anim.json", &clipBase);

    for (size_t i = 0; i < maxNPCCount; ++i) {
        uint32_t type = rnd.NextRange(0, 11) * 4;
        instancedSpriteRenderers[i] = new ME::SpriteRenderer(0, 0, 1, 2, type);

        // Flip if on left side.
        if (instancedSpriteTransforms[i]->GetPosition().x > 0) {
            instancedSpriteRenderers[i]->ToggleFlipHorizontal(true);
        }

        ME::SpriteAnimator* animator0 = new ME::SpriteAnimator(instancedSpriteRenderers[i], 6);
        instancedSpriteRenderers[i]->animator = animator0;

        SpriteAnimClip* clip = ME::SpriteAnimClip::DuplicateWithOffset(clipBase, static_cast<int16_t>(type));

        animator0->AddClip(clip);
        animator0->ChangeClip(0);

        spriteInstanceData[i] = new ME::SpriteRendererInstanceData();
        spriteInstanceData[i]->modelMatrixData = instancedSpriteTransforms[i]->GetModelMatrix().GetDataForShader();
        spriteInstanceData[i]->atlasIndex = 0;
        spriteInstanceData[i]->color = ME::Color::White();
    }
    delete clipBase;

    instancedSpriteRendererCount = maxNPCCount;
}
