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
    Scene::Init();
}

void ME::SceneCharacterTest::CreateResources() {
    Scene::CreateResources();

    quadPaths[0] = "meshes/quad.obj";
    quadCount = 1;

    textureCount = 0;

    spriteTexturePaths[0] = "textures/characters/princess.png";
    spriteTexturePaths[1] = "textures/enemies/enemy_atlas.png";
    spriteTexturePaths[2] = "textures/sprites/fireball.png";
    spriteTextureCount = 3;

    shaderPaths[0] = "shaders/metal/sprite.metal";
    shaderPaths[1] = "shaders/metal/sprite_instanced.metal";
    shaderCount = 2;

    ME::JsonUtils::LoadTextureAtlasProps("texture_data/atlas_princess.json", textureAtlasProperties[0]);
    ME::JsonUtils::LoadTextureAtlasProps("texture_data/atlas_enemy.json", textureAtlasProperties[1]);
    ME::JsonUtils::LoadTextureAtlasProps("texture_data/atlas_fireball.json", textureAtlasProperties[2]);
    textureAtlasPropertiesCount = 3;

    sfxPaths[0] = "audio/sfx/laser.wav";
    sfxCount = 1;
    musicPaths[0] = "audio/music/ambience.wav";
    musicCount = 1;
}

void ME::SceneCharacterTest::BuildLights() {
    Scene::BuildLights();
}

void ME::SceneCharacterTest::BuildCamera() {
    Scene::BuildCamera();
    spriteCamera->orthographicSize = 50.0f;
}

void ME::SceneCharacterTest::BuildTransforms() {
    Scene::BuildTransforms();
}

void ME::SceneCharacterTest::BuildMeshRenderers() {
    Scene::BuildMeshRenderers();
}

void ME::SceneCharacterTest::BuildSpriteTransforms() {
    Scene::BuildSpriteTransforms();

    // Character Sprite
    AddSpriteTransform(ME::Vec3::Zero, ME::Vec3(charWidth, charHeight, 1.0f));

    dynamicColliders[dynamicColliderCount] =
        ME::ColliderAABB(dynamicColliderCount, true, false, PhysicsLayer::Player, PhysicsLayer::Enemy,
                         *spriteTransforms[0], charCollScaleMult);
    ++dynamicColliderCount;
}

void ME::SceneCharacterTest::BuildSpriteRenderers() {
    Scene::BuildSpriteRenderers();

    ME::SpriteRenderer* spRend = new ME::SpriteRenderer(0, 0, 0, 0, 1, ME::Color::White());

    ME::SpriteAnimator* animator0 = new ME::SpriteAnimator(spRend, 8);
    spRend->animator = animator0;

    ME::SpriteAnimClip* clipUp = nullptr;
    ME::JsonUtils::LoadSpriteAnimClipFromJSON("anim/char_princess_up.json", &clipUp);
    SpriteAnimClip* clipLeft = ME::SpriteAnimClip::DuplicateWithOffset(clipUp, static_cast<int16_t>(9));
    SpriteAnimClip* clipDown = ME::SpriteAnimClip::DuplicateWithOffset(clipUp, static_cast<int16_t>(18));
    SpriteAnimClip* clipRight = ME::SpriteAnimClip::DuplicateWithOffset(clipUp, static_cast<int16_t>(27));

    animator0->AddClip(clipUp);
    animator0->AddClip(clipRight);
    animator0->AddClip(clipDown);
    animator0->AddClip(clipLeft);

    AddSpriteRenderer(spRend);
}

void ME::SceneCharacterTest::BuildInstancedSpriteTransforms() {
    Scene::BuildInstancedSpriteTransforms();

    // NPC Sprites.
    ME::Random rnd{"npc_position", true};
    for (size_t i = 0; i < maxNPCCount; ++i) {
        float x = 9000.0f;
        float y = 9000.0f;

        // Adding to instance buffer 0;
        AddInstancedSpriteTransform(ME::Vec3(x, y, 0.0f), ME::Vec3(npcWidth, npcHeight, 1.0f), 0);

        dynamicColliders[dynamicColliderCount] = ME::ColliderAABB(
            dynamicColliderCount, true, false, PhysicsLayer::Enemy, (PhysicsLayer::Player | PhysicsLayer::Projectile),
            *instancedSpriteTransforms0[i], enemyCollScaleMult);
        ++dynamicColliderCount;
    }

    // Bullet Sprites.
    ME::Random rnd2{"bullet_position", true};
    for (size_t i = 0; i < maxBulletCount; ++i) {
        float x = -9000.0f;
        float y = -9000.0f;
        // Adding to instance buffer 1;
        AddInstancedSpriteTransform(ME::Vec3(x, y, 0.0f), ME::Vec3(bulletSize, bulletSize, 1.0f), 1);

        dynamicColliders[dynamicColliderCount] =
            ME::ColliderAABB(dynamicColliderCount, true, false, PhysicsLayer::Projectile, PhysicsLayer::Enemy,
                             *instancedSpriteTransforms1[i], bulletCollScaleMult);
        ++dynamicColliderCount;
    }
}

void ME::SceneCharacterTest::BuildInstancedSpriteRenderers() {
    Scene::BuildInstancedSpriteRenderers();

    ME::Random rnd{"npc_type", true};
    ME::SpriteAnimClip* clipBase = nullptr;  // Used as base to duplicate with offset. Deleted afterwards.
    ME::JsonUtils::LoadSpriteAnimClipFromJSON("anim/enemy_base_anim.json", &clipBase);

    for (size_t i = 0; i < maxNPCCount; ++i) {
        uint32_t type = rnd.NextRange(0, 11) * 4;
        ME::SpriteRenderer* spRend = new ME::SpriteRenderer(0, 0, 1, 2, static_cast<uint16_t>(type));

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
