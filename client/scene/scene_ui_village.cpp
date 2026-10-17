#include "scene_ui_village.h"

#include "moksha/client/ui/label.h"
#include "moksha/client/ui/panel.h"
#include "moksha/client/utils/json_utils.h"

VS::SceneUIVillage::~SceneUIVillage() {
    delete titleLabel;
    delete titleBar;
}

void VS::SceneUIVillage::CreateResources() {
    SceneUI::CreateResources();

    // The engine's UI atlas and font.
    spriteTexturePaths[0] = "textures/ui/ui_atlas.dds";
    spriteTexturePaths[1] = "textures/font/ascii_ibm_transparent.dds";
    spriteTexturePaths.count = 2;

    ME::JsonUtils::LoadTextureAtlasProps("texture_data/atlas_ui.json", textureAtlasProperties[0]);
    ME::JsonUtils::LoadTextureAtlasProps("texture_data/font_atlas_01.json", textureAtlasProperties[1]);
    textureAtlasProperties.count = 2;
}

void VS::SceneUIVillage::BuildUIElements() {
    SceneUI::BuildUIElements();

    titleBar = new ME::Panel(0, 0, 0, 0, 48);
    titleBar->SetAnchor(ME::UIAnchor::TopCenter);
    titleBar->SetPivot(ME::UIPivot::TopCenter);
    titleBar->SetOffset(ME::Vec2{0.0f, 10.0f});
    titleBar->SetSize(ME::Vec2{600.0f, 60.0f});
    titleBar->Init();
    AddUIElement(titleBar);

    titleLabel =
        new ME::Label("The Village Shop", 0, 1, 0, ME::Color::White(), 32, 32, 2, 2, 80, ME::TextAlignment::Center);
    titleLabel->SetAnchor(ME::UIAnchor::Center);
    titleLabel->SetPivot(ME::UIPivot::Center);
    titleLabel->SetSize(ME::Vec2{560.0f, 40.0f});
    titleLabel->SetParent(titleBar);
    titleLabel->Init();
    AddUIElement(titleLabel);
}
