#include "scene_ui.h"

#include <cstddef>

#include "../utils/json_utils.h"
#include "src/misc/game_constants.h"

ME::SceneUI::SceneUI() {}

ME::SceneUI::~SceneUI() {
    delete[] spriteTexturePaths;

    for (uint32_t i = 0; i < uiSpriteRendererCount; ++i) {
        delete uiSpriteTransforms[i];
        delete uiSpriteRenderers[i];
        delete uiSpriteInstanceData[i];
    }
    delete[] uiSpriteTransforms;
    delete[] uiSpriteRenderers;
    delete[] uiSpriteInstanceData;

    for (uint32_t i = 0; i < textTransformsCount; ++i) {
        delete textTransforms[i];
    }
    delete[] textTransforms;

    for (uint32_t i = 0; i < textRendererCount; ++i) {
        delete textRenderers[i];
    }
    delete[] textRenderers;

    for (uint32_t i = 0; i < textInstanceDataCount; ++i) {
        delete textInstanceData[i];
    }
    delete[] textInstanceData;
}

void ME::SceneUI::Update() {
    UpdateUISpriteRenderers();
    UpdateTextRenderers();
}

void ME::SceneUI::Init() {
    CreateResources();
    BuildUISprites();
    BuildTextRenderers();
}

void ME::SceneUI::CreateResources() {
    spriteTexturePaths = new const char*[Constants::MaxSpriteTextureCount];
    textureAtlasProperties = new ME::TextureAtlasProperties[Constants::MaxTextureAtlasPropertiesCount];

    uiSpriteTransforms = new ME::Transform*[Constants::MaxUISpriteTransformCount];
    uiSpriteRenderers = new ME::SpriteRenderer*[Constants::MaxUISpriteRendererCount];
    uiSpriteInstanceData = new ME::UISpriteRendererInstanceData*[Constants::MaxUISpriteInstanceDataCount];
    textTransforms = new ME::Transform*[Constants::MaxTextTransformsCount];
    textRenderers = new ME::TextRenderer*[Constants::MaxTextRendererCount];
    textInstanceData = new ME::TextRendererInstanceData*[Constants::MaxTextInstanceDataCount];

    // UI Sprite paths.
    spriteTexturePaths[0] = "textures/ui/ui_atlas.png";
    spriteTexturePaths[1] = "textures/font/ascii_ibm_transparent.png";
    spriteTextureCount = 2;

    ME::JsonUtils::LoadTextureAtlasProps("texture_data/atlas_ui.json", textureAtlasProperties[0]);
    ME::JsonUtils::LoadTextureAtlasProps("texture_data/font_atlas_01.json", textureAtlasProperties[1]);
    textureAtlasPropertiesCount = 2;
}

void ME::SceneUI::BuildUISprites() {
    const uint8_t uiSpriteCount = 15;
    const uint8_t uiSpriteWidthCount = 5;
    const float uiSpriteSize = 80.0f;

    for (size_t i = 0; i < uiSpriteCount; ++i) {
        uint8_t row = static_cast<uint8_t>(i / uiSpriteWidthCount);
        uint8_t col = static_cast<uint8_t>(i % uiSpriteWidthCount);
        float x = -175.0f + col * uiSpriteSize;
        float y = -500.0f + row * uiSpriteSize;

        uiSpriteTransforms[i] = new ME::Transform();
        uiSpriteTransforms[i]->SetPosition(x, y, 0.0f);
        uiSpriteTransforms[i]->SetScale(uiSpriteSize, uiSpriteSize);
    }

    uint16_t s = 399;
    uint16_t panel[15] = {static_cast<uint16_t>(s + 60), static_cast<uint16_t>(s + 61), static_cast<uint16_t>(s + 61),
                          static_cast<uint16_t>(s + 61), static_cast<uint16_t>(s + 62), static_cast<uint16_t>(s + 30),
                          static_cast<uint16_t>(s + 31), static_cast<uint16_t>(s + 31), static_cast<uint16_t>(s + 31),
                          static_cast<uint16_t>(s + 32), static_cast<uint16_t>(s + 0),  static_cast<uint16_t>(s + 1),
                          static_cast<uint16_t>(s + 1),  static_cast<uint16_t>(s + 1),  static_cast<uint16_t>(s + 2)};

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

void ME::SceneUI::BuildTextRenderers() {
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

void ME::SceneUI::UpdateUISpriteRenderers() {
    // Updating transforms and atlas indicesfor dirty instances.
    // In separate loops to avoid cache misses.

    for (uint32_t i = 0; i < uiSpriteRendererCount; ++i) {
        if (!uiSpriteRenderers[i]->bDirty) {
            continue;
        }
        uiSpriteInstanceData[i]->modelMatrixData = uiSpriteTransforms[i]->GetModelMatrix().GetDataForShader();
    }

    for (uint32_t i = 0; i < uiSpriteRendererCount; ++i) {
        if (!uiSpriteRenderers[i]->bDirty) {
            continue;
        }
        uiSpriteInstanceData[i]->atlasIndex = uiSpriteRenderers[i]->atlasIndex;
        uiSpriteInstanceData[i]->color = uiSpriteRenderers[i]->color;
        uiSpriteInstanceData[i]->flags = uiSpriteRenderers[i]->flags;
    }

    for (uint32_t i = 0; i < uiSpriteRendererCount; ++i) {
        if (uiSpriteRenderers[i]->bDirty) {
            uiSpriteRenderers[i]->bDirty = false;
        }
    }
}

void ME::SceneUI::UpdateTextRenderers() {
    uint32_t count = 0;
    for (uint32_t i = 0; i < textRendererCount; ++i) {
        if (!textRenderers[i]->bDirty) {
            count += textRenderers[i]->GetCount();
            continue;
        }
        for (int j = 0; j < textRenderers[i]->GetCount(); ++j) {
            textInstanceData[count]->atlasIndex = textRenderers[i]->text[j];
            count++;
        }
        textRenderers[i]->bDirty = false;
    }
}
