#include "game.h"

#include <cmath>
#include <iostream>

#include "../Input/InputManager.h"

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
