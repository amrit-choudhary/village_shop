#include "scene_ui_hud.h"

#include <cstddef>

#include "../utils/json_utils.h"
#include "src/misc/game_constants.h"

ME::SceneUIHUD::SceneUIHUD() {}

ME::SceneUIHUD::~SceneUIHUD() {}

void ME::SceneUIHUD::Init() {
    SceneUI::Init();
}

void ME::SceneUIHUD::CreateResources() {
    SceneUI::CreateResources();

    // UI Sprite paths.
    spriteTexturePaths[0] = "textures/ui/ui_atlas.png";
    spriteTexturePaths[1] = "textures/font/ascii_ibm_transparent.png";
    spriteTextureCount = 2;

    ME::JsonUtils::LoadTextureAtlasProps("texture_data/atlas_ui.json", textureAtlasProperties[0]);
    ME::JsonUtils::LoadTextureAtlasProps("texture_data/font_atlas_01.json", textureAtlasProperties[1]);
    textureAtlasPropertiesCount = 2;
}

void ME::SceneUIHUD::BuildUISprites() {
    SceneUI::BuildUISprites();

    const uint8_t uiHeaderCount = 7;
    const float uiHeaderSize = 90.0f;
    uint16_t sp = 48;
    uint16_t headSP[7] = {static_cast<uint16_t>(sp + 0), static_cast<uint16_t>(sp + 1), static_cast<uint16_t>(sp + 1),
                          static_cast<uint16_t>(sp + 1), static_cast<uint16_t>(sp + 1), static_cast<uint16_t>(sp + 1),
                          static_cast<uint16_t>(sp + 2)};

    for (size_t i = 0; i < uiHeaderCount; ++i) {
        float x = -290.0f + i * uiHeaderSize;
        float y = 405.0f;

        ME::SpriteRenderer* sprRend = new ME::SpriteRenderer(0, 0, 1, 1, headSP[i]);
        AddUISprite(ME::Vec3{x, y, 0.0f}, ME::Vec3{uiHeaderSize, uiHeaderSize, 1.0f}, sprRend);
    }

    const uint8_t uiSpriteCount = 15;
    const uint8_t uiSpriteWidthCount = 5;
    const float uiSpriteSize = 80.0f;

    const uint8_t j = uiHeaderCount;

    uint16_t s = 399;
    uint16_t panel[15] = {static_cast<uint16_t>(s + 60), static_cast<uint16_t>(s + 61), static_cast<uint16_t>(s + 61),
                          static_cast<uint16_t>(s + 61), static_cast<uint16_t>(s + 62), static_cast<uint16_t>(s + 30),
                          static_cast<uint16_t>(s + 31), static_cast<uint16_t>(s + 31), static_cast<uint16_t>(s + 31),
                          static_cast<uint16_t>(s + 32), static_cast<uint16_t>(s + 0),  static_cast<uint16_t>(s + 1),
                          static_cast<uint16_t>(s + 1),  static_cast<uint16_t>(s + 1),  static_cast<uint16_t>(s + 2)};

    for (size_t i = 0; i < uiSpriteCount; ++i) {
        uint8_t row = static_cast<uint8_t>(i / uiSpriteWidthCount);
        uint8_t col = static_cast<uint8_t>(i % uiSpriteWidthCount);
        float x = -175.0f + col * uiSpriteSize;
        float y = -560.0f + row * uiSpriteSize;

        ME::SpriteRenderer* sprRend = new ME::SpriteRenderer(0, 0, 1, 1, panel[i]);
        AddUISprite(ME::Vec3{x, y, 0.0f}, ME::Vec3{uiSpriteSize, uiSpriteSize, 1.0f}, sprRend);
    }

    uiSpriteInstanceDataCount = uiSpriteRendererCount;
}

void ME::SceneUIHUD::BuildTextRenderers() {
    SceneUI::BuildTextRenderers();

    ME::TextRenderer* textRend1 = new ME::TextRenderer{"Game Name", 0, 2, 0, ME::Color{"#dfeaec"}, 40, 40, -10, 0, 0};
    float x1 = -(textRend1->GetRenderWidth() / 2.0f);
    float y1 = 408.0f;
    AddUIText(ME::Vec3{x1, y1, 0.0f}, ME::Vec3{(float)textRend1->width, (float)textRend1->height, 1.0f}, textRend1);

    ME::TextRenderer* textRend2 = new ME::TextRenderer{"Score: 0000", 0, 2, 0, ME::Color{"#3a5975"}, 40, 40, -10, 0, 0};
    float x2 = -(textRend2->GetRenderWidth() / 2.0f);
    float y2 = -410.0f;
    AddUIText(ME::Vec3{x2, y2, 0.0f}, ME::Vec3{(float)textRend2->width, (float)textRend2->height, 1.0f}, textRend2);
}
