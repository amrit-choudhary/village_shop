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

    instancedSpriteTransforms0 = new ME::Transform*[Constants::MaxInstancedSpriteTransformCount];
    instancedSpriteRenderers0 = new ME::SpriteRenderer*[Constants::MaxInstancedSpriteRendererCount];
    spriteInstanceData0 = new ME::SpriteRendererInstanceData*[Constants::MaxInstancedSpriteRendererCount];

    instancedSpriteTransforms1 = new ME::Transform*[Constants::MaxInstancedSpriteTransformCount];
    instancedSpriteRenderers1 = new ME::SpriteRenderer*[Constants::MaxInstancedSpriteRendererCount];
    spriteInstanceData1 = new ME::SpriteRendererInstanceData[Constants::MaxInstancedSpriteRendererCount];

    staticColliders = new ME::ColliderAABB[Constants::MaxStaticColliderCount];
    dynamicColliders = new ME::ColliderAABB[Constants::MaxDynamicColliderCount];

    meshCount = 0;

    quadPaths[0] = "meshes/quad.obj";
    quadCount = 1;

    textureCount = 0;

    spriteTexturePaths[0] = "textures/characters/character_walk_8.png";
    spriteTexturePaths[1] = "textures/enemies/enemy_atlas.png";
    spriteTexturePaths[2] = "textures/sprites/fireball.png";
    spriteTextureCount = 3;

    shaderPaths[0] = "shaders/metal/sprite.metal";
    shaderPaths[1] = "shaders/metal/sprite_instanced.metal";
    shaderCount = 2;

    ME::JsonUtils::LoadTextureAtlasProps("texture_data/atlas_char_8.json", textureAtlasProperties[0]);
    ME::JsonUtils::LoadTextureAtlasProps("texture_data/atlas_enemy.json", textureAtlasProperties[1]);
    ME::JsonUtils::LoadTextureAtlasProps("texture_data/atlas_fireball.json", textureAtlasProperties[2]);
    textureAtlasPropertiesCount = 3;

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
    // NPC Sprites.
    ME::Random rnd{"npc_position", true};
    for (size_t i = 0; i < maxNPCCount; ++i) {
        float x = rnd.NextDouble() * 70.0f - 35.0f;
        float y = rnd.NextDouble() * 80.0f - 40.0f;

        instancedSpriteTransforms0[i] = new ME::Transform();
        instancedSpriteTransforms0[i]->SetPosition(x, y, 0.0f);
        instancedSpriteTransforms0[i]->SetScale(npcWidth, npcHeight);
    }
    instancedSpriteTransformCount0 = maxNPCCount;

    // Bullet Sprites.
    ME::Random rnd2{"bullet_position", true};
    for (size_t i = 0; i < maxBulletCount; ++i) {
        float x = rnd2.NextDouble() * 500.0f - 250.0f;
        float y = rnd2.NextDouble() * 500.0f - 250.0f;

        instancedSpriteTransforms1[i] = new ME::Transform();
        instancedSpriteTransforms1[i]->SetPosition(x, y, 0.0f);
        instancedSpriteTransforms1[i]->SetScale(bulletSize, bulletSize);
    }
    instancedSpriteTransformCount1 = maxBulletCount;
}

void ME::SceneCharacterTest::BuildInstancedSpriteRenderers() {
    ME::Random rnd{"npc_type", true};
    ME::SpriteAnimClip* clipBase = nullptr;  // Used as base to duplicate with offset. Deleted afterwards.
    ME::JsonUtils::LoadSpriteAnimClipFromJSON("anim/enemy_base_anim.json", &clipBase);

    for (size_t i = 0; i < maxNPCCount; ++i) {
        uint32_t type = rnd.NextRange(0, 11) * 4;
        instancedSpriteRenderers0[i] = new ME::SpriteRenderer(0, 0, 1, 2, type);

        // Flip if on left side.
        if (instancedSpriteTransforms0[i]->GetPosition().x > 0) {
            instancedSpriteRenderers0[i]->ToggleFlipHorizontal(true);
        }

        ME::SpriteAnimator* animator0 = new ME::SpriteAnimator(instancedSpriteRenderers0[i], 6);
        instancedSpriteRenderers0[i]->animator = animator0;
        SpriteAnimClip* clip = ME::SpriteAnimClip::DuplicateWithOffset(clipBase, static_cast<int16_t>(type));

        animator0->AddClip(clip);
        animator0->ChangeClip(0);

        spriteInstanceData0[i] = new ME::SpriteRendererInstanceData();
        spriteInstanceData0[i]->modelMatrixData = instancedSpriteTransforms0[i]->GetModelMatrix().GetDataForShader();
        spriteInstanceData0[i]->atlasIndex = 0;
        spriteInstanceData0[i]->color = ME::Color::White();
    }
    delete clipBase;
    instancedSpriteRendererCount0 = maxNPCCount;

    // Bullets.

    for (size_t i = 0; i < maxBulletCount; ++i) {
        instancedSpriteRenderers1[i] = new ME::SpriteRenderer(0, 0, 1, 2, (i % 6));

        // Flip if on left side.
        if (instancedSpriteTransforms1[i]->GetPosition().x > 0) {
            instancedSpriteRenderers1[i]->ToggleFlipHorizontal(true);
        }

        spriteInstanceData1[i].modelMatrixData = instancedSpriteTransforms1[i]->GetModelMatrix().GetDataForShader();
        spriteInstanceData1[i].atlasIndex = 0;
        spriteInstanceData1[i].color = ME::Color::White();
    }

    instancedSpriteRendererCount1 = maxBulletCount;
}
