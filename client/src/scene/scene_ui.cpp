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
    uiSpriteInstanceData = new ME::UISpriteRendererInstanceData[Constants::MaxUISpriteInstanceDataCount];
    textTransforms = new ME::Transform*[Constants::MaxTextTransformsCount];
    textRenderers = new ME::TextRenderer*[Constants::MaxTextRendererCount];
    textInstanceData = new ME::TextRendererInstanceData[Constants::MaxTextInstanceDataCount];

    // UI Sprite paths.
    spriteTexturePaths[0] = "textures/ui/ui_atlas.png";
    spriteTexturePaths[1] = "textures/font/ascii_ibm_transparent.png";
    spriteTextureCount = 2;

    ME::JsonUtils::LoadTextureAtlasProps("texture_data/atlas_ui.json", textureAtlasProperties[0]);
    ME::JsonUtils::LoadTextureAtlasProps("texture_data/font_atlas_01.json", textureAtlasProperties[1]);
    textureAtlasPropertiesCount = 2;
}

void ME::SceneUI::BuildUISprites() {
    const uint8_t uiHeaderCount = 7;
    const float uiHeaderSize = 90.0f;

    for (size_t i = 0; i < uiHeaderCount; ++i) {
        float x = -290.0f + i * uiHeaderSize;

        uiSpriteTransforms[i] = new ME::Transform();
        uiSpriteTransforms[i]->SetPosition(x, 405.0f, 0.0f);
        uiSpriteTransforms[i]->SetScale(uiHeaderSize, uiHeaderSize);
    }

    uint16_t sp = 48;
    uint16_t headSP[7] = {static_cast<uint16_t>(sp + 0), static_cast<uint16_t>(sp + 1), static_cast<uint16_t>(sp + 1),
                          static_cast<uint16_t>(sp + 1), static_cast<uint16_t>(sp + 1), static_cast<uint16_t>(sp + 1),
                          static_cast<uint16_t>(sp + 2)};

    for (size_t i = 0; i < uiHeaderCount; ++i) {
        uiSpriteRenderers[i] = new ME::SpriteRenderer(0, 0, 1, 1, headSP[i]);

        uiSpriteInstanceData[i].modelMatrixData = uiSpriteTransforms[i]->GetModelMatrix().GetDataForShader();
        uiSpriteInstanceData[i].atlasIndex = i;
        uiSpriteInstanceData[i].color = ME::Color::White();
    }

    const uint8_t uiSpriteCount = 15;
    const uint8_t uiSpriteWidthCount = 5;
    const float uiSpriteSize = 80.0f;

    const uint8_t j = uiHeaderCount;

    for (size_t i = 0; i < uiSpriteCount; ++i) {
        uint8_t row = static_cast<uint8_t>(i / uiSpriteWidthCount);
        uint8_t col = static_cast<uint8_t>(i % uiSpriteWidthCount);
        float x = -175.0f + col * uiSpriteSize;
        float y = -560.0f + row * uiSpriteSize;

        uiSpriteTransforms[i + j] = new ME::Transform();
        uiSpriteTransforms[i + j]->SetPosition(x, y, 0.0f);
        uiSpriteTransforms[i + j]->SetScale(uiSpriteSize, uiSpriteSize);
    }

    uint16_t s = 399;
    uint16_t panel[15] = {static_cast<uint16_t>(s + 60), static_cast<uint16_t>(s + 61), static_cast<uint16_t>(s + 61),
                          static_cast<uint16_t>(s + 61), static_cast<uint16_t>(s + 62), static_cast<uint16_t>(s + 30),
                          static_cast<uint16_t>(s + 31), static_cast<uint16_t>(s + 31), static_cast<uint16_t>(s + 31),
                          static_cast<uint16_t>(s + 32), static_cast<uint16_t>(s + 0),  static_cast<uint16_t>(s + 1),
                          static_cast<uint16_t>(s + 1),  static_cast<uint16_t>(s + 1),  static_cast<uint16_t>(s + 2)};

    for (size_t i = 0; i < uiSpriteCount; ++i) {
        uiSpriteRenderers[i + j] = new ME::SpriteRenderer(0, 0, 1, 1, panel[i]);

        uiSpriteInstanceData[i + j].modelMatrixData = uiSpriteTransforms[i + j]->GetModelMatrix().GetDataForShader();
        uiSpriteInstanceData[i + j].atlasIndex = i;
        uiSpriteInstanceData[i + j].color = ME::Color::White();
    }

    uiSpriteRendererCount = uiHeaderCount + uiSpriteCount;
    uiSpriteInstanceDataCount = uiHeaderCount + uiSpriteCount;
}

void ME::SceneUI::BuildTextRenderers() {
    // Text 1
    ME::TextRenderer* textRend1 =
        new ME::TextRenderer{"Game Name         ", 0, 2, 0, ME::Color{"#dfeaec"}, 40, 40, -10, 0, 0};
    textRenderers[0] = textRend1;

    textTransforms[0] = new ME::Transform();
    textTransforms[0]->SetPosition(-(textRend1->GetRenderWidth() / 2.0f), 408.0f, 0.0f);
    textTransforms[0]->SetScale(textRend1->width, textRend1->height);

    for (uint32_t i = 0; i < textRend1->GetCount(); ++i) {
        ME::Transform transform;
        ME::Vec3 position = textTransforms[0]->GetPosition();
        position.x += (i * (textRend1->width + textRend1->letterSpacing));
        transform.SetPosition(position);
        transform.SetScale(textRend1->width, textRend1->height);
        textInstanceData[i].modelMatrixData = transform.GetModelMatrix().GetDataForShader();
        textInstanceData[i].color = textRend1->color;
        textInstanceData[i].atlasIndex = textRend1->text[i];
    }

    // Text 2
    ME::TextRenderer* textRend2 = new ME::TextRenderer{"Score: 0000", 0, 2, 0, ME::Color{"#3a5975"}, 40, 40, -10, 0, 0};
    textRenderers[1] = textRend2;

    textTransforms[1] = new ME::Transform();
    textTransforms[1]->SetPosition(-(textRend2->GetRenderWidth() / 2.0f), -410.0f, 0.0f);
    textTransforms[1]->SetScale(textRend2->width, textRend2->height);

    uint32_t j = textRend1->GetCount();

    for (uint32_t i = 0; i < textRend2->GetCount(); ++i) {
        ME::Transform transform;
        ME::Vec3 position = textTransforms[1]->GetPosition();
        position.x += (i * (textRend2->width + textRend2->letterSpacing));
        transform.SetPosition(position);
        transform.SetScale(textRend2->width, textRend2->height);
        textInstanceData[i + j].modelMatrixData = transform.GetModelMatrix().GetDataForShader();
        textInstanceData[i + j].color = textRend2->color;
        textInstanceData[i + j].atlasIndex = textRend2->text[i];
    }

    textRendererCount = 2;
    textTransformsCount = 2;
    textInstanceDataCount = textRend1->GetCount() + textRend2->GetCount();
}

void ME::SceneUI::UpdateUISpriteRenderers() {
    // Updating transforms and atlas indicesfor dirty instances.
    // In separate loops to avoid cache misses.

    for (uint32_t i = 0; i < uiSpriteRendererCount; ++i) {
        if (!uiSpriteRenderers[i]->bDirty) {
            continue;
        }
        uiSpriteInstanceData[i].modelMatrixData = uiSpriteTransforms[i]->GetModelMatrix().GetDataForShader();
    }

    for (uint32_t i = 0; i < uiSpriteRendererCount; ++i) {
        if (!uiSpriteRenderers[i]->bDirty) {
            continue;
        }
        uiSpriteInstanceData[i].atlasIndex = uiSpriteRenderers[i]->atlasIndex;
        uiSpriteInstanceData[i].color = uiSpriteRenderers[i]->color;
        uiSpriteInstanceData[i].flags = uiSpriteRenderers[i]->flags;
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
            textInstanceData[count].atlasIndex = textRenderers[i]->text[j];
            count++;
        }
        textRenderers[i]->bDirty = false;
    }
}
