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
    ME::Scene::Init();
}

void ME::SceneCharacterTest::CreateResources() {
    ME::Scene::CreateResources();

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
}

void ME::SceneCharacterTest::BuildLights() {
    ME::Scene::BuildLights();
}

void ME::SceneCharacterTest::BuildCamera() {
    ME::Scene::BuildCamera();
    spriteCamera->orthographicSize = 50.0f;
}

void ME::SceneCharacterTest::BuildTransforms() {
    ME::Scene::BuildTransforms();
}

void ME::SceneCharacterTest::BuildMeshRenderers() {
    ME::Scene::BuildMeshRenderers();
}

void ME::SceneCharacterTest::BuildSpriteTransforms() {
    ME::Scene::BuildSpriteTransforms();
    // Character Sprite
    AddSpriteTransform(ME::Vec3::Zero, ME::Vec3(charWidth, charHeight, 1.0f));
}

void ME::SceneCharacterTest::BuildSpriteRenderers() {
    ME::Scene::BuildSpriteRenderers();

    ME::SpriteRenderer* spRend = new ME::SpriteRenderer(0, 0, 0, 0, 1, ME::Color::White());

    ME::SpriteAnimator* animator0 = new ME::SpriteAnimator(spRend, 8);
    spRend->animator = animator0;

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

    AddSpriteRenderer(spRend);
}

void ME::SceneCharacterTest::BuildInstancedSpriteTransforms() {
    ME::Scene::BuildInstancedSpriteTransforms();

    // NPC Sprites.
    ME::Random rnd{"npc_position", true};
    for (size_t i = 0; i < maxNPCCount; ++i) {
        float x = rnd.NextDouble() * 70.0f - 35.0f;
        float y = rnd.NextDouble() * 80.0f - 40.0f;
        // Adding to instance buffer 0;
        AddInstancedSpriteTransform(ME::Vec3(x, y, 0.0f), ME::Vec3(npcWidth, npcHeight, 1.0f), 0);
    }

    // Bullet Sprites.
    ME::Random rnd2{"bullet_position", true};
    for (size_t i = 0; i < maxBulletCount; ++i) {
        float x = 9000.0f;
        float y = 9000.0f;
        // Adding to instance buffer 1;
        AddInstancedSpriteTransform(ME::Vec3(x, y, 0.0f), ME::Vec3(bulletSize, bulletSize, 1.0f), 1);
    }
}

void ME::SceneCharacterTest::BuildInstancedSpriteRenderers() {
    ME::Scene::BuildInstancedSpriteRenderers();

    ME::Random rnd{"npc_type", true};
    ME::SpriteAnimClip* clipBase = nullptr;  // Used as base to duplicate with offset. Deleted afterwards.
    ME::JsonUtils::LoadSpriteAnimClipFromJSON("anim/enemy_base_anim.json", &clipBase);

    for (size_t i = 0; i < maxNPCCount; ++i) {
        uint32_t type = rnd.NextRange(0, 11) * 4;
        ME::SpriteRenderer* spRend = new ME::SpriteRenderer(0, 0, 1, 2, type);

        // Flip if on left side.
        if (instancedSpriteTransforms0[i]->GetPosition().x > 0) {
            spRend->ToggleFlipHorizontal(true);
        }

        ME::SpriteAnimator* animator0 = new ME::SpriteAnimator(spRend, 6);
        spRend->animator = animator0;
        SpriteAnimClip* clip = ME::SpriteAnimClip::DuplicateWithOffset(clipBase, static_cast<int16_t>(type));

        animator0->AddClip(clip);
        animator0->ChangeClip(0);

        AddInstancedSpriteRenderer(spRend, 0);
    }
    delete clipBase;

    // Bullets.

    for (size_t i = 0; i < maxBulletCount; ++i) {
        ME::SpriteRenderer* spRend = new ME::SpriteRenderer(0, 0, 1, 2, (i % 6));

        // Flip if on left side.
        if (instancedSpriteTransforms1[i]->GetPosition().x > 0) {
            spRend->ToggleFlipHorizontal(true);
        }

        AddInstancedSpriteRenderer(spRend, 1);
    }
}
