#include "game_ui_demo.h"

#include <cstdio>

#include "src/debug/debug_system.h"

ME::GameUIDemo::GameUIDemo() : Game() {}

ME::GameUIDemo::~GameUIDemo() {}

void ME::GameUIDemo::Init(ME::Time::TimeManager* currentTimeManager) {
    Game::Init(currentTimeManager);

    // Plain, un-subclassed Scene: no Build*() overrides, so no meshes/sprites/lights — renders
    // as a black background.
    scene = new ME::Scene();
    scene->Init();

    // SceneUIDemo::Init() builds the entire widget tree via its BuildUIElements() override —
    // see scene_ui_demo.cpp. uiScene (base Game's SceneUI*) and sceneUIDemo alias the same
    // object; sceneUIDemo just keeps the concrete type for GetHealthLabel()/GetScoreLabel().
    sceneUIDemo = new ME::SceneUIDemo();
    sceneUIDemo->Init();
    uiScene = sceneUIDemo;

    ME::Log("UI Demo Game Start!");
}

void ME::GameUIDemo::Start() {
    Game::Start();
}

void ME::GameUIDemo::Update(double deltaTime) {
    Game::Update(deltaTime);

    bool healthChanged = false;
    bool scoreChanged = false;

    if (inputManager->GetKeyPressed(ME::Input::KeyCode::D)) {
        ++health;
        healthChanged = true;
    }
    if (inputManager->GetKeyPressed(ME::Input::KeyCode::A)) {
        --health;
        healthChanged = true;
    }
    if (inputManager->GetKeyPressed(ME::Input::KeyCode::W)) {
        ++score;
        scoreChanged = true;
    }
    if (inputManager->GetKeyPressed(ME::Input::KeyCode::S)) {
        --score;
        scoreChanged = true;
    }

    if (healthChanged) {
        char buf[32];
        snprintf(buf, sizeof(buf), "Health: %d", health);
        sceneUIDemo->GetHealthLabel()->SetText(buf);
        DebugSystem::ScreenPrintSlot(1, buf);
    }
    if (scoreChanged) {
        char buf[32];
        snprintf(buf, sizeof(buf), "Score: %d", score);
        sceneUIDemo->GetScoreLabel()->SetText(buf);
        DebugSystem::ScreenPrint(buf, 2.0f);
    }

    // Continuous WASD movement for the center image (held, not edge-triggered, unlike the
    // health/score keys above) — UI space is Y-down, so W/up decreases y and S/down increases it.
    constexpr float imageMoveSpeed = 200.0f;
    float moveDelta = static_cast<float>(imageMoveSpeed * deltaTime);
    if (inputManager->GetKeyDown(ME::Input::KeyCode::D)) {
        imageOffset.x += moveDelta;
    }
    if (inputManager->GetKeyDown(ME::Input::KeyCode::A)) {
        imageOffset.x -= moveDelta;
    }
    if (inputManager->GetKeyDown(ME::Input::KeyCode::S)) {
        imageOffset.y += moveDelta;
    }
    if (inputManager->GetKeyDown(ME::Input::KeyCode::W)) {
        imageOffset.y -= moveDelta;
    }
    sceneUIDemo->GetCenterImage()->SetOffset(imageOffset);
}

void ME::GameUIDemo::End() {
    // sceneUIDemo and uiScene alias the same object — deleting uiScene destroys the widgets
    // SceneUIDemo owns (see SceneUIDemo::~SceneUIDemo()).
    delete uiScene;
    delete scene;

    Game::End();
    ME::Log("UI Demo Game End!");
}
