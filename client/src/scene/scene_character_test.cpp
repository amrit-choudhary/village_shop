#include "scene_character_test.h"

#include <cstddef>

#include "../anim/sprite_anim_clip.h"
#include "../anim/sprite_animator.h"
#include "../utils/json_utils.h"
#include "src/misc/game_constants.h"

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
    spriteTextureCount = 2;

    shaderPaths[0] = "shaders/metal/sprite.metal";
    shaderPaths[1] = "shaders/metal/sprite_instanced.metal";
    shaderCount = 2;

    ME::JsonUtils::LoadTextureAtlasProps("texture_data/font_atlas_01.json", textureAtlasProperties[0]);
    ME::JsonUtils::LoadTextureAtlasProps("texture_data/atlas_char_8.json", textureAtlasProperties[1]);
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

void ME::SceneCharacterTest::BuildSpriteTransforms() {}

void ME::SceneCharacterTest::BuildSpriteRenderers() {}

void ME::SceneCharacterTest::BuildInstancedSpriteTransforms() {
    // Character Sprite
    instancedSpriteTransforms[0] = new ME::Transform();
    instancedSpriteTransforms[0]->SetPosition(0.0f, 0.0f, 0.0f);
    instancedSpriteTransforms[0]->SetScale(charWidth, charHeight);

    instancedSpriteTransformCount = 1;
}

void ME::SceneCharacterTest::BuildInstancedSpriteRenderers() {
    instancedSpriteRenderers[0] = new ME::SpriteRenderer(0, 0, 1, 1, ME::Color::White());

    ME::SpriteAnimator* animator0 = new ME::SpriteAnimator(8);
    instancedSpriteRenderers[0]->animator = animator0;

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

    spriteInstanceData[0] = new ME::SpriteRendererInstanceData();
    spriteInstanceData[0]->modelMatrixData = instancedSpriteTransforms[0]->GetModelMatrix().GetDataForShader();
    spriteInstanceData[0]->atlasIndex = 0;
    spriteInstanceData[0]->color = ME::Color::White();

    instancedSpriteRendererCount = 1;
}

void ME::SceneCharacterTest::BuildTextRenderers() {
    // Text 1
    ME::TextRenderer* textRend1 =
        new ME::TextRenderer{"\x0F Hero Test \x0F", 0, 2, 0, ME::Color::White(), 6, 6, -1, 0, 0};
    textRenderers[0] = textRend1;

    textTransforms[0] = new ME::Transform();
    textTransforms[0]->SetPosition(-(textRend1->GetRenderWidth() / 2.0f), 45.0f, 0.0f);
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
}
