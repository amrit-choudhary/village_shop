#include "game.h"

#include <cmath>
#include <iostream>

#include "../input/input_manager.h"

using namespace ME;
using namespace ME::Input;

Game::Game() {}

Game::~Game() {}

void Game::Init(ME::Time::TimeManager* currentTimeManager) {
    ME::Log("Game Start!");

    timeManager = currentTimeManager;
}

void Game::Update(double deltaTime) {}

void Game::End() {
    std::cout << "Game Run Time: " << timeManager->GetTimeSinceStartup() << '\n';
    std::cout << "Average FPS: " << timeManager->GetFrameCount() / timeManager->GetTimeSinceStartup() << '\n';
    std::cout << "Average Uncapped FPS: " << timeManager->GetNotFFRFrameCount() / timeManager->GetTimeSinceStartup()
              << '\n';
    std::cout << "Total Frames: " << timeManager->GetFrameCount() << '\n';

    ME::Log("Game Over!");
}

void ME::Game::SetInputManagerRef(ME::Input::InputManager* ptrInput) {
    inputManager = ptrInput;
}

void ME::Game::SetConnectionRef(ME::Connection* ptrConnection) {
    connection = ptrConnection;
}

void ME::Game::SetPhysicsSystemRef(ME::PhysicsSystem* ptrPhysicsSystem) {
    physicsSystem = ptrPhysicsSystem;
}

void ME::Game::SetAnimationSystemRef(ME::AnimationSystem* ptrAnimationSystem) {
    animationSystem = ptrAnimationSystem;
}

void ME::Game::SetAudioSystemRef(ME::AudioSystem* ptrAudioSystem) {
    audioSystem = ptrAudioSystem;
}

void ME::Game::CollisionCallback(ColliderAABB* a, ColliderAABB* b, CollisionResultAABB* result) {
    delete result;
}

ME::Scene* ME::Game::GetScene() {
    return scene;
}

ME::SceneUI* ME::Game::GetUIScene() {
    return uiScene;
}
