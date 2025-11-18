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
    BuildUISpriteTransforms();
    BuildUISpriteRenderers();
    BuildTextRenderers();
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
    uiSpriteTransforms = new ME::Transform*[Constants::MaxUISpriteTransformCount];
    uiSpriteRenderers = new ME::SpriteRenderer*[Constants::MaxUISpriteRendererCount];
    uiSpriteInstanceData = new ME::UISpriteRendererInstanceData*[Constants::MaxUISpriteInstanceDataCount];
    textTransforms = new ME::Transform*[Constants::MaxTextTransformsCount];
    textRenderers = new ME::TextRenderer*[Constants::MaxTextRendererCount];
    textInstanceData = new ME::TextRendererInstanceData*[Constants::MaxTextInstanceDataCount];

    staticColliders = new ME::ColliderAABB[Constants::MaxStaticColliderCount];
    dynamicColliders = new ME::ColliderAABB[Constants::MaxDynamicColliderCount];

    meshCount = 0;

    quadPaths[0] = "meshes/quad.obj";
    quadCount = 1;

    textureCount = 0;

    spriteTexturePaths[0] = "textures/font/ascii_ibm_transparent.png";
    spriteTexturePaths[1] = "textures/characters/character_walk_8.png";
    spriteTexturePaths[2] = "textures/enemies/enemy_atlas.png";
    spriteTexturePaths[3] = "textures/ui/ui_atlas.png";
    spriteTextureCount = 4;

    shaderPaths[0] = "shaders/metal/sprite.metal";
    shaderPaths[1] = "shaders/metal/sprite_instanced.metal";
    shaderCount = 2;

    ME::JsonUtils::LoadTextureAtlasProps("texture_data/font_atlas_01.json", textureAtlasProperties[0]);
    ME::JsonUtils::LoadTextureAtlasProps("texture_data/atlas_char_8.json", textureAtlasProperties[1]);
    ME::JsonUtils::LoadTextureAtlasProps("texture_data/atlas_enemy.json", textureAtlasProperties[2]);
    ME::JsonUtils::LoadTextureAtlasProps("texture_data/atlas_ui.json", textureAtlasProperties[3]);
    textureAtlasPropertiesCount = 4;

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
    spriteRenderers[0] = new ME::SpriteRenderer(0, 0, 1, 1, 1, ME::Color::White());

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

void ME::SceneCharacterTest::BuildUISpriteTransforms() {
    for (size_t i = 0; i < uiSpriteCount; ++i) {
        uint8_t row = static_cast<uint8_t>(i / uiSpriteWidthCount);
        uint8_t col = static_cast<uint8_t>(i % uiSpriteWidthCount);
        float x = -175.0f + col * uiSpriteSize;
        float y = -500.0f + row * uiSpriteSize;

        uiSpriteTransforms[i] = new ME::Transform();
        uiSpriteTransforms[i]->SetPosition(x, y, 0.0f);
        uiSpriteTransforms[i]->SetScale(uiSpriteSize, uiSpriteSize);
    }
    uiSpriteTransformCount = uiSpriteCount;
}

void ME::SceneCharacterTest::BuildUISpriteRenderers() {
    uint16_t s = 399;
    uint16_t panel[15] = {s + 60, s + 61, s + 61, s + 61, s + 62, s + 30, s + 31, s + 31,
                          s + 31, s + 32, s + 0,  s + 1,  s + 1,  s + 1,  s + 2};
    for (size_t i = 0; i < uiSpriteCount; ++i) {
        uiSpriteRenderers[i] = new ME::SpriteRenderer(3, 0, 1, 1, panel[i]);

        uiSpriteInstanceData[i] = new ME::UISpriteRendererInstanceData();
        uiSpriteInstanceData[i]->modelMatrixData = uiSpriteTransforms[i]->GetModelMatrix().GetDataForShader();
        uiSpriteInstanceData[i]->atlasIndex = i;
        uiSpriteInstanceData[i]->color = ME::Color::White();
    }
    uiSpriteRendererCount = uiSpriteCount;
    uiSpriteInstanceDataCount = uiSpriteCount;
}

void ME::SceneCharacterTest::BuildTextRenderers() {
    // Text 1
    ME::TextRenderer* textRend1 = new ME::TextRenderer{"Score: 0000", 0, 2, 0, ME::Color{"#3a5975"}, 40, 40, -10, 0, 0};
    textRenderers[0] = textRend1;

    textTransforms[0] = new ME::Transform();
    textTransforms[0]->SetPosition(-(textRend1->GetRenderWidth() / 2.0f), -375.0f, 0.0f);
    textTransforms[0]->SetScale(textRend1->width, textRend1->height);

    for (uint32_t i = 0; i < textRend1->GetCount(); ++i) {
        textInstanceData[i] = new ME::TextRendererInstanceData();

        ME::Transform transform;
        ME::Vec3 position = textTransforms[0]->GetPosition();
        position.x += (i * (textRend1->width + textRend1->letterSpacing));
        transform.SetPosition(position);
        transform.SetScale(textRend1->width, textRend1->height);
        textInstanceData[i]->modelMatrixData = transform.GetModelMatrix().GetDataForShader();

        textInstanceData[i]->color = textRend1->color;
        textInstanceData[i]->atlasIndex = textRend1->text[i];
    }

    textInstanceDataCount = textRend1->GetCount();
    textRendererCount = 1;
}
