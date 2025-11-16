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
    BuildTextRenderers();
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
    spriteTexturePaths[1] = "textures/sprites/dice_white.png";
    spriteTexturePaths[2] = "textures/sprites/dice_pink.png";
    spriteTextureCount = 3;

    shaderPaths[0] = "shaders/metal/sprite.metal";
    shaderPaths[1] = "shaders/metal/sprite_instanced.metal";
    shaderCount = 2;

    ME::JsonUtils::LoadTextureAtlasProps("texture_data/font_atlas_01.json", textureAtlasProperties[0]);
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
    instancedSpriteRenderers[0] = new ME::SpriteRenderer(0, 0, 1, 1, ME::Color::White());

    ME::SpriteAnimator* animator0 = new ME::SpriteAnimator(instancedSpriteRenderers[0], 8);
    instancedSpriteRenderers[0]->animator = animator0;

    ME::SpriteAnimClip* clip0 = nullptr;
    ME::JsonUtils::LoadSpriteAnimClipFromJSON("anim/sprite_anim_01.json", &clip0);
    animator0->AddClip(clip0);

    spriteInstanceData[0] = new ME::SpriteRendererInstanceData();
    spriteInstanceData[0]->modelMatrixData = instancedSpriteTransforms[0]->GetModelMatrix().GetDataForShader();
    spriteInstanceData[0]->atlasIndex = 0;
    spriteInstanceData[0]->color = ME::Color::White();

    instancedSpriteRenderers[1] = new ME::SpriteRenderer(0, 0, 2, 1, ME::Color::White());

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
void ME::SceneDiceSimple::BuildTextRenderers() {
    // Text 1
    ME::TextRenderer* textRend1 =
        new ME::TextRenderer{"\x0F DICE GAME \x0F", 0, 2, 0, ME::Color::White(), 6, 6, -1, 0, 0};
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

    // Text 2
    ME::TextRenderer* textRend2 =
        new ME::TextRenderer{"Rolling...", 0, 2, 0, ME::Color::RandomColorPretty(), 6, 6, -1, 0, 0};

    textRenderers[1] = textRend2;
    textTransforms[1] = new ME::Transform();
    textTransforms[1]->SetPosition(-(textRend2->GetRenderWidth() / 2.0f), -40.0f, 0.0f);
    textTransforms[1]->SetScale(textRend2->width, textRend2->height);

    uint32_t j = textInstanceDataCount;
    for (uint32_t i = 0; i < textRend2->GetCount(); ++i) {
        textInstanceData[i + j] = new ME::TextRendererInstanceData();

        ME::Transform transform;
        ME::Vec3 position = textTransforms[1]->GetPosition();
        position.x += (i * (textRend2->width + textRend2->letterSpacing));
        transform.SetPosition(position);
        transform.SetScale(textRend2->width, textRend2->height);
        textInstanceData[i + j]->modelMatrixData = transform.GetModelMatrix().GetDataForShader();

        textInstanceData[i + j]->color = textRend2->color;
        textInstanceData[i + j]->atlasIndex = textRend2->text[i];
    }

    textInstanceDataCount += textRend2->GetCount();

    // Text 3
    ME::TextRenderer* textRend3 =
        new ME::TextRenderer{"YOU LOSE", 0, 2, 0, ME::Color::RandomColorPretty(), 10, 10, -1, 0, 0};
    textRenderers[2] = textRend3;
    textTransforms[2] = new ME::Transform();
    textTransforms[2]->SetPosition(-(textRend3->GetRenderWidth() / 2.0f), 0.0f, 0.0f);
    textTransforms[2]->SetScale(textRend3->width, textRend3->height);

    uint32_t k = textInstanceDataCount;
    for (uint32_t i = 0; i < textRend3->GetCount(); ++i) {
        textInstanceData[i + k] = new ME::TextRendererInstanceData();

        ME::Transform transform;
        ME::Vec3 position = textTransforms[2]->GetPosition();
        position.x += (i * (textRend3->width + textRend3->letterSpacing));
        transform.SetPosition(position);
        transform.SetScale(textRend3->width, textRend3->height);
        textInstanceData[i + k]->modelMatrixData = transform.GetModelMatrix().GetDataForShader();

        textInstanceData[i + k]->color = textRend3->color;
        textInstanceData[i + k]->atlasIndex = textRend3->text[i];
    }

    textInstanceDataCount += textRend3->GetCount();

    // Text 4
    ME::TextRenderer* textRend4 = new ME::TextRenderer{"AI", 0, 2, 0, ME::Color::RandomColorPretty(), 4, 4, -1, 0, 0};
    textRenderers[3] = textRend4;
    textTransforms[3] = new ME::Transform();
    textTransforms[3]->SetPosition(-(textRend4->GetRenderWidth() / 2.0f) - 15.0f, diceGap, 0.0f);
    textTransforms[3]->SetScale(textRend4->width, textRend4->height);
    uint32_t m = textInstanceDataCount;
    for (uint32_t i = 0; i < textRend4->GetCount(); ++i) {
        textInstanceData[i + m] = new ME::TextRendererInstanceData();

        ME::Transform transform;
        ME::Vec3 position = textTransforms[3]->GetPosition();
        position.x += (i * (textRend4->width + textRend4->letterSpacing));
        transform.SetPosition(position);
        transform.SetScale(textRend4->width, textRend4->height);
        textInstanceData[i + m]->modelMatrixData = transform.GetModelMatrix().GetDataForShader();

        textInstanceData[i + m]->color = textRend4->color;
        textInstanceData[i + m]->atlasIndex = textRend4->text[i];
    }

    textInstanceDataCount += textRend4->GetCount();

    // Text 5
    ME::TextRenderer* textRend5 = new ME::TextRenderer{"YOU", 0, 2, 0, ME::Color::RandomColorPretty(), 4, 4, -1, 0, 0};
    textRenderers[4] = textRend5;
    textTransforms[4] = new ME::Transform();
    textTransforms[4]->SetPosition(-(textRend5->GetRenderWidth() / 2.0f) - 15.0f, -diceGap, 0.0f);
    textTransforms[4]->SetScale(textRend5->width, textRend5->height);
    uint32_t n = textInstanceDataCount;
    for (uint32_t i = 0; i < textRend5->GetCount(); ++i) {
        textInstanceData[i + n] = new ME::TextRendererInstanceData();

        ME::Transform transform;
        ME::Vec3 position = textTransforms[4]->GetPosition();
        position.x += (i * (textRend5->width + textRend5->letterSpacing));
        transform.SetPosition(position);
        transform.SetScale(textRend5->width, textRend5->height);
        textInstanceData[i + n]->modelMatrixData = transform.GetModelMatrix().GetDataForShader();

        textInstanceData[i + n]->color = textRend5->color;
        textInstanceData[i + n]->atlasIndex = textRend5->text[i];
    }
    textInstanceDataCount += textRend5->GetCount();

    textRendererCount = 5;
    textTransformsCount = 5;
}
