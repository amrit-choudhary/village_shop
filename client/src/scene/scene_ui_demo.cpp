#include "scene_ui_demo.h"

#include "src/utils/json_utils.h"

ME::SceneUIDemo::SceneUIDemo() {}

ME::SceneUIDemo::~SceneUIDemo() {
    delete healthLabel;
    delete scoreLabel;
    delete centerImage;
    delete titleLabel;
    delete topBarPanel;
}

void ME::SceneUIDemo::CreateResources() {
    SceneUI::CreateResources();

    spriteTexturePaths[0] = "textures/ui/ui_atlas.dds";
    spriteTexturePaths[1] = "textures/font/ascii_ibm_transparent.dds";
    spriteTextureCount = 2;

    ME::JsonUtils::LoadTextureAtlasProps("texture_data/atlas_ui.json", textureAtlasProperties[0]);
    ME::JsonUtils::LoadTextureAtlasProps("texture_data/font_atlas_01.json", textureAtlasProperties[1]);
    textureAtlasPropertiesCount = 2;
}

void ME::SceneUIDemo::BuildUIElements() {
    SceneUI::BuildUIElements();

    topBarPanel = new ME::Panel(0, 0, 0, 0, 48);
    topBarPanel->SetAnchor(ME::UIAnchor::TopCenter);
    topBarPanel->SetPivot(ME::UIPivot::TopCenter);
    topBarPanel->SetOffset(ME::Vec2{0.0f, 10.0f});
    topBarPanel->SetSize(ME::Vec2{600.0f, 60.0f});
    topBarPanel->Init();
    AddUIElement(topBarPanel);

    titleLabel = new ME::Label("Village Shop UI Test", 0, 1, 0, ME::Color::White(), 32, 20, 2, 2, 80);
    titleLabel->SetAnchor(ME::UIAnchor::Center);
    titleLabel->SetPivot(ME::UIPivot::Center);
    titleLabel->SetOffset(ME::Vec2{0.0f, 0.0f});
    titleLabel->SetSize(ME::Vec2{300.0f, 40.0f});
    titleLabel->SetParent(topBarPanel);
    titleLabel->Init();
    AddUIElement(titleLabel);

    centerImage = new ME::Image(0, 0, 0, 0, 399);
    centerImage->SetAnchor(ME::UIAnchor::Center);
    centerImage->SetPivot(ME::UIPivot::Center);
    centerImage->SetOffset(ME::Vec2{0.0f, 0.0f});
    centerImage->SetSize(ME::Vec2{300.0f, 150.0f});
    centerImage->Init();
    AddUIElement(centerImage);

    healthLabel = new ME::Label("Health: 100", 0, 1, 0, ME::Color::White(), 16, 10, 2, 2, 80);
    healthLabel->SetAnchor(ME::UIAnchor::BottomRight);
    healthLabel->SetPivot(ME::UIPivot::BottomRight);
    healthLabel->SetOffset(ME::Vec2{-20.0f, -20.0f});
    healthLabel->SetSize(ME::Vec2{160.0f, 24.0f});
    healthLabel->Init();
    AddUIElement(healthLabel);

    scoreLabel = new ME::Label("Score: 0", 0, 1, 0, ME::Color::White(), 16, 10, 2, 2, 80);
    scoreLabel->SetAnchor(ME::UIAnchor::BottomLeft);
    scoreLabel->SetPivot(ME::UIPivot::BottomLeft);
    scoreLabel->SetOffset(ME::Vec2{20.0f, -20.0f});
    scoreLabel->SetSize(ME::Vec2{160.0f, 24.0f});
    scoreLabel->Init();
    AddUIElement(scoreLabel);
}

ME::Label* ME::SceneUIDemo::GetHealthLabel() const {
    return healthLabel;
}

ME::Label* ME::SceneUIDemo::GetScoreLabel() const {
    return scoreLabel;
}

ME::Image* ME::SceneUIDemo::GetCenterImage() const {
    return centerImage;
}
