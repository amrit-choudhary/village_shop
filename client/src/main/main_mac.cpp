#include "main_mac.h"

#include <iostream>

#define NS_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#define MTK_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION

#include <AppKit/AppKit.hpp>
#include <Metal/Metal.hpp>
#include <MetalKit/MetalKit.hpp>

ME::GameMain::GameMain() {}

ME::GameMain::~GameMain() {
    game.End();
    connection.End();
    inputManager.End();
    timeManager.End();
    // renderer.End();
    physicsSystem.End();
}

void ME::GameMain::SetViewAndDevice(void* view, void* device) {}

void ME::GameMain::Init() {
    // Read game params from file.
    INIMap iniMap = Load();
    fps = std::atoi(iniMap["settings"]["fps"].c_str());
    maxRunTime = std::atoi(iniMap["settings"]["maxRunTime"].c_str());

    // Init global variables.
    timeManager.Init(fps, false);
    bool shouldTick = false;
    double deltaTime = 0.0f;

    inputManager.Init();
    connection.Init();
    physicsSystem.Init();

    game.SetInputManagerRef(&inputManager);
    game.SetConnectionRef(&connection);
    game.SetPhysicsSystemRef(&physicsSystem);
    game.Init(&timeManager);

    // renderer.InitMTL(device, view);
    // renderer.SetScene(game.GetScene());
}

void ME::GameMain::Update() {
    bool shouldTick = timeManager.Update();
    double deltaTime = 0.0f;

    if (shouldTick) {
        deltaTime = timeManager.GetDeltaTime();

        inputManager.Update(deltaTime);

        game.Update(deltaTime);

        // renderer.Update();
        // renderer.Draw(view);

        connection.Update(deltaTime);

        physicsSystem.Update(deltaTime);
    }

    // Perform Rendering
    // The Renderer::DrawFrame method needs to be adapted to use the
    // pView's currentRenderPassDescriptor and currentDrawable.
    // _renderer.DrawFrame(pView);  // Example: Pass the view to the renderer

    // Check for exit condition (e.g., max run time)
    if (maxRunTime > 0 && timeManager.GetTimeSinceStartup() > maxRunTime) {
        ShutDownGameSystems();
        // NS::Application::sharedApplication()->windows()->object<NS::Window>(0)->close();
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
    // renderer.End();
    physicsSystem.End();
}