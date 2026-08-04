#ifdef VG_WIN

#include "main_win.h"

#include <cstdio>
#include <iostream>

ME::GameMain::GameMain() {}

ME::GameMain::~GameMain() {
    game.End();
    connection.End();
    inputManager.End();
    timeManager.End();
    physicsSystem.End();
    animationSystem.End();
    audioSystem.End();
    uiSystem.End();
    renderer.End();

    ME::DebugSystem::SetInstance(nullptr);
    debugSystem.End();
}

void ME::GameMain::Init(HWND hWnd) {
    this->hWnd = hWnd;

    // Read game params from file.
    INIMap iniMap = Load();
    fixedFrameRate = std::atoi(iniMap["settings"]["fixedFrameRate"].c_str());
    vsync = std::atoi(iniMap["settings"]["vsync"].c_str()) != 0;

    debugSystem.Init();
    ME::DebugSystem::SetInstance(&debugSystem);

    inputManager.Init();
    winInputManager = static_cast<ME::Input::InputManagerWin*>(inputManager.GetPlatformInputManager());
    connection.Init();
    physicsSystem.Init();
    animationSystem.Init();
    audioSystem.Init();

    game.SetInputManagerRef(&inputManager);
    game.SetConnectionRef(&connection);
    game.SetPhysicsSystemRef(&physicsSystem);
    game.SetAnimationSystemRef(&animationSystem);
    game.SetAudioSystemRef(&audioSystem);
    game.Init(&timeManager);

    uiSystem.Init();
    uiSystem.SetUIScene(game.GetUIScene());
    game.SetUISystemRef(&uiSystem);

    renderer.InitDX(hWnd);
    renderer.SetVsyncEnabled(vsync);
    renderer.SetScenes(game.GetScene(), game.GetUIScene());
    renderer.SetDebugUIScene(debugSystem.GetUIScene());

    audioSystem.SetScene(game.GetScene());

    // Clock init after all systems are initialized.
    ME::Time::TimeConfig timeConfig;
    timeConfig.fixedStepFPS = static_cast<double>(fixedFrameRate);
    timeManager.Init(timeConfig);

    // Start the game.
    game.Start();
}

void ME::GameMain::HandleInput(UINT msg, WPARAM wParam, LPARAM lParam) {
    if (winInputManager != nullptr) {
        winInputManager->HandleInput(msg, wParam, lParam);
    }
}

void ME::GameMain::Update() {
    timeManager.BeginFrame();
    double deltaTime = timeManager.GetFrameDeltaTime();

    char fpsBuf[64];
    snprintf(fpsBuf, sizeof(fpsBuf), "FPS: %.0f", timeManager.GetCurrentFPS());
    ME::DebugSystem::ScreenPrintSlot(0, fpsBuf);

    inputManager.PreUpdate();
    inputManager.Update(deltaTime);

    for (int i = 0; i < timeManager.GetPendingFixedSteps(); ++i) {
        double fixedDeltaTime = timeManager.GetFixedDeltaTime();

        game.FixedUpdate(fixedDeltaTime);
        physicsSystem.Update(fixedDeltaTime);
        animationSystem.Update(fixedDeltaTime);
    }

    game.Update(deltaTime);
    uiSystem.Update(deltaTime);
    debugSystem.Update(deltaTime);

    inputManager.PostUpdate();

    renderer.Update();
    renderer.Draw();

    connection.Update(deltaTime);
    audioSystem.Update(deltaTime);
}

void ME::GameMain::Exit() {
    // Clean up game systems here
    std::cout << "Game exited." << std::endl;
}

void ME::GameMain::ShutDownGameSystems() {
    game.End();
    connection.End();
    inputManager.End();
    timeManager.End();
    physicsSystem.End();
    animationSystem.End();
    audioSystem.End();
    uiSystem.End();
    renderer.End();

    ME::DebugSystem::SetInstance(nullptr);
    debugSystem.End();
}

#endif  // VG_WIN
