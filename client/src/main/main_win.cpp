#ifdef VG_WIN

#include "main_win.h"

#include <iostream>

ME::GameMain::GameMain() {}

ME::GameMain::~GameMain() {
    game.End();
    connection.End();
    inputManager.End();
    timeManager.End();
    physicsSystem.End();
    animationSystem.End();
    renderer.End();
}

void ME::GameMain::Init(HWND hWnd) {
    this->hWnd = hWnd;

    // Read game params from file.
    INIMap iniMap = Load();
    fps = std::atoi(iniMap["settings"]["fps"].c_str());
    maxRunTime = std::atoi(iniMap["settings"]["maxRunTime"].c_str());

    inputManager.Init();
    winInputManager = static_cast<ME::Input::InputManagerWin*>(inputManager.GetPlatformInputManager());
    connection.Init();
    physicsSystem.Init();
    animationSystem.Init();

    game.SetInputManagerRef(&inputManager);
    game.SetConnectionRef(&connection);
    game.SetPhysicsSystemRef(&physicsSystem);
    game.SetAnimationSystemRef(&animationSystem);
    game.Init(&timeManager);

    renderer.InitDX(hWnd);
    renderer.SetScene(game.GetScene());

    // Clock init after all systems are initialized.
    timeManager.Init(fps, false);
    bool shouldTick = false;
    double deltaTime = 0.0f;
}

void ME::GameMain::HandleInput(UINT msg, WPARAM wParam, LPARAM lParam) {
    if (winInputManager != nullptr) {
        winInputManager->HandleInput(msg, wParam, lParam);
    }
}

void ME::GameMain::Update() {
    bool shouldTick = timeManager.Update();
    double deltaTime = 0.0f;

    if (shouldTick) {
        deltaTime = timeManager.GetDeltaTime();

        inputManager.Update(deltaTime);

        game.Update(deltaTime);

        renderer.Update();
        renderer.Draw();

        connection.Update(deltaTime);

        physicsSystem.Update(deltaTime);

        animationSystem.Update(deltaTime);
    }

    // Perform Rendering

    // Check for exit condition (e.g., max run time)
    if (maxRunTime > 0 && timeManager.GetTimeSinceStartup() > maxRunTime) {
        ShutDownGameSystems();
    }
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
    renderer.End();
}

#endif  // VG_WIN
