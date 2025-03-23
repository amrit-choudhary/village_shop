#include "game.h"

#include <cmath>
#include <iostream>

#include "../Input/InputManager.h"

using namespace ME::Input;

Game::Game() {
    index = 0;
    ringBuffer = new ME::RingBuffer<uint8_t>(10);
}

Game::~Game() { delete ringBuffer; }

void Game::Init(ME::Time::TimeManager* currentTimeManager) {
    ME::Log("Game Start!");

    timeManager = currentTimeManager;
}

void Game::Update(double deltaTime) {
    for (int i = 0; i < BUFFER_X * BUFFER_Y; ++i) {
        healths[i].Update(deltaTime);
    }

    for (int i = 0; i < BUFFER_X * BUFFER_Y; ++i) {
        hungers[i].Update(deltaTime);
    }

    for (int i = 0; i < BUFFER_X * BUFFER_Y; ++i) {
        golds[i].Update(deltaTime);
    }

    static double offset = 0;
    offset += deltaTime * 10;

    for (int y = 0; y < BUFFER_Y; ++y) {
        for (int x = 0; x < BUFFER_X; ++x) {
            uint32_t index = y * BUFFER_Y + x;
            double val = (hungers[index].Hunger / 100.0f) * ASCII_COUNT_SHORT;

            buffer1[x][y] = ASCII_COUNT_SHORT - (uint8_t)val;
        }
    }

    if (InputManager::GetKeyDown(KeyCode::W) || InputManager::GetKeyDown(KeyCode::UArrow)) {
        y -= deltaTime * 10;
    }
    if (InputManager::GetKeyDown(KeyCode::S) || InputManager::GetKeyDown(KeyCode::DArrow)) {
        y += deltaTime * 10;
    }
    if (InputManager::GetKeyDown(KeyCode::A) || InputManager::GetKeyDown(KeyCode::LArrow)) {
        x -= deltaTime * 10;
    }
    if (InputManager::GetKeyDown(KeyCode::D) || InputManager::GetKeyDown(KeyCode::RArrow)) {
        x += deltaTime * 10;
    }

    buffer1[x][y] = buffer1[x + 1][y] = buffer1[x][y + 1] = buffer1[x + 1][y + 1] = 11;
}

void Game::End() {
    std::cout << "Game Run Time: " << timeManager->GetTimeSinceStartup() << '\n';
    std::cout << "Average FPS: " << timeManager->GetFrameCount() / timeManager->GetTimeSinceStartup() << '\n';
    std::cout << "Average Uncapped FPS: " << timeManager->GetNotFFRFrameCount() / timeManager->GetTimeSinceStartup()
              << '\n';
    std::cout << "Total Frames: " << timeManager->GetFrameCount() << '\n';

    ME::Log("Game Over!");
}
