#include "scene_ui_net_test.h"

#include "client/src/utils/json_utils.h"

ME::SceneUINetTest::SceneUINetTest() {}

ME::SceneUINetTest::~SceneUINetTest() {
    delete scoreButtonLabel;
    delete scoreButtonPanel;
    delete scoreButton;
    delete opponentScoreLabel;
    delete yourScoreLabel;
    delete pingLabel;
    delete statusLabel;
    delete titleLabel;
    delete topBarPanel;
}

void ME::SceneUINetTest::CreateResources() {
    SceneUI::CreateResources();

    spriteTexturePaths[0] = "textures/ui/ui_atlas.dds";
    spriteTexturePaths[1] = "textures/font/ascii_ibm_transparent.dds";
    spriteTexturePaths.count = 2;

    ME::JsonUtils::LoadTextureAtlasProps("texture_data/atlas_ui.json", textureAtlasProperties[0]);
    ME::JsonUtils::LoadTextureAtlasProps("texture_data/font_atlas_01.json", textureAtlasProperties[1]);
    textureAtlasProperties.count = 2;
}

void ME::SceneUINetTest::BuildUIElements() {
    SceneUI::BuildUIElements();

    topBarPanel = new ME::Panel(0, 0, 0, 0, 48);
    topBarPanel->SetAnchor(ME::UIAnchor::TopCenter);
    topBarPanel->SetPivot(ME::UIPivot::TopCenter);
    topBarPanel->SetOffset(ME::Vec2{0.0f, 10.0f});
    topBarPanel->SetSize(ME::Vec2{600.0f, 60.0f});
    topBarPanel->Init();
    AddUIElement(topBarPanel);

    titleLabel =
        new ME::Label("Network Test", 0, 1, 0, ME::Color::White(), 32, 32, 2, 2, 80, ME::TextAlignment::Center);
    titleLabel->SetAnchor(ME::UIAnchor::Center);
    titleLabel->SetPivot(ME::UIPivot::Center);
    titleLabel->SetOffset(ME::Vec2{0.0f, 0.0f});
    titleLabel->SetSize(ME::Vec2{300.0f, 40.0f});
    titleLabel->SetParent(topBarPanel);
    titleLabel->Init();
    AddUIElement(titleLabel);

    statusLabel =
        new ME::Label("Connecting...", 0, 1, 0, ME::Color::Yellow(), 24, 24, 0, 2, 80, ME::TextAlignment::Center);
    statusLabel->SetAnchor(ME::UIAnchor::Center);
    statusLabel->SetPivot(ME::UIPivot::Center);
    statusLabel->SetOffset(ME::Vec2{0.0f, -60.0f});
    statusLabel->SetSize(ME::Vec2{400.0f, 40.0f});
    statusLabel->Init();
    AddUIElement(statusLabel);

    pingLabel = new ME::Label("Pings: 0 / Pongs: 0", 0, 1, 0, ME::Color::White(), 24, 24, 0, 2, 80,
                              ME::TextAlignment::Center);
    pingLabel->SetAnchor(ME::UIAnchor::Center);
    pingLabel->SetPivot(ME::UIPivot::Center);
    pingLabel->SetOffset(ME::Vec2{0.0f, -10.0f});
    pingLabel->SetSize(ME::Vec2{400.0f, 40.0f});
    pingLabel->Init();
    AddUIElement(pingLabel);

    yourScoreLabel =
        new ME::Label("Your Score: 0", 0, 1, 0, ME::Color::Green(), 24, 24, 0, 2, 80, ME::TextAlignment::Left);
    yourScoreLabel->SetAnchor(ME::UIAnchor::BottomLeft);
    yourScoreLabel->SetPivot(ME::UIPivot::BottomLeft);
    yourScoreLabel->SetOffset(ME::Vec2{20.0f, -20.0f});
    yourScoreLabel->SetSize(ME::Vec2{260.0f, 40.0f});
    yourScoreLabel->Init();
    AddUIElement(yourScoreLabel);

    opponentScoreLabel = new ME::Label("Opponent Score: -", 0, 1, 0, ME::Color::Yellow(), 24, 24, 0, 2, 80,
                                       ME::TextAlignment::Right);
    opponentScoreLabel->SetAnchor(ME::UIAnchor::BottomRight);
    opponentScoreLabel->SetPivot(ME::UIPivot::BottomRight);
    opponentScoreLabel->SetOffset(ME::Vec2{-20.0f, -20.0f});
    opponentScoreLabel->SetSize(ME::Vec2{260.0f, 40.0f});
    opponentScoreLabel->Init();
    AddUIElement(opponentScoreLabel);

    scoreButton = new ME::Button();
    scoreButton->SetAnchor(ME::UIAnchor::BottomCenter);
    scoreButton->SetPivot(ME::UIPivot::BottomCenter);
    scoreButton->SetOffset(ME::Vec2{0.0f, -20.0f});
    scoreButton->SetSize(ME::Vec2{200.0f, 50.0f});
    scoreButton->Init();
    AddUIElement(scoreButton);

    scoreButtonPanel = new ME::Panel(0, 0, 0, 0, 660);
    scoreButtonPanel->SetAnchor(ME::UIAnchor::Center);
    scoreButtonPanel->SetPivot(ME::UIPivot::Center);
    scoreButtonPanel->SetSize(ME::Vec2{200.0f, 50.0f});
    scoreButtonPanel->SetParent(scoreButton);
    scoreButtonPanel->Init();
    AddUIElement(scoreButtonPanel);

    scoreButtonLabel =
        new ME::Label("Score +1", 0, 1, 0, ME::Color::Black(), 24, 24, 0, 2, 40, ME::TextAlignment::Center);
    scoreButtonLabel->SetAnchor(ME::UIAnchor::Center);
    scoreButtonLabel->SetPivot(ME::UIPivot::Center);
    scoreButtonLabel->SetSize(ME::Vec2{200.0f, 50.0f});
    scoreButtonLabel->SetParent(scoreButton);
    scoreButtonLabel->Init();
    AddUIElement(scoreButtonLabel);

    scoreButton->SetPanel(scoreButtonPanel);
}

ME::Label* ME::SceneUINetTest::GetStatusLabel() const {
    return statusLabel;
}

ME::Label* ME::SceneUINetTest::GetPingLabel() const {
    return pingLabel;
}

ME::Label* ME::SceneUINetTest::GetYourScoreLabel() const {
    return yourScoreLabel;
}

ME::Label* ME::SceneUINetTest::GetOpponentScoreLabel() const {
    return opponentScoreLabel;
}

ME::Button* ME::SceneUINetTest::GetScoreButton() const {
    return scoreButton;
}
