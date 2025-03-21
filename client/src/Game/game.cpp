#include "game.h"

#include <cmath>
#include <iostream>

#include "../Input/InputManager.h"

using namespace VG::Input;

Game::Game() {
    index = 0;
    ringBuffer = new VG::RingBuffer<uint8_t>(10);
}

Game::~Game() { delete ringBuffer; }

void Game::Init(VG::Time::TimeManager* currentTimeManager) {
    VG::Log("Game Start!");

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

    // std::cout << "X: " << x << "Y: " << y << '\n';

    buffer1[x][y] = buffer1[x + 1][y] = buffer1[x][y + 1] = buffer1[x + 1][y + 1] = 11;

    ringBuffer->Insert(rnd.NextRange(65, 66));
    ++index;

    std::cout << '\n';
    for (int i = 0; i < ringBuffer->GetCount(); ++i) {
        std::cout << ringBuffer->Get(i) << ", ";
    }

    uint8_t lut[10] = {0, 0, 0, 1, 1, 1, 2, 2, 2, 3};
    VG::RandomWt rndWt(12345, lut);

    int acount = 0;
    int bcount = 0;
    int ccount = 0;
    int dcount = 0;
    int ecount = 0;
    for (int i = 0; i < 1'000'000; ++i) {
        uint32_t res = rndWt.Next() + 'A';
        switch (res) {
            case 'A':
                ++acount;
                break;
            case 'B':
                ++bcount;
                break;
            case 'C':
                ++ccount;
                break;
            case 'D':
                ++dcount;
                break;
            case 'E':
                ++ecount;
                break;
            default:
                break;
        }
    }
    std::cout << "\n" << acount << ", " << bcount << ", " << ccount << ", " << dcount << ", " << ecount << "\n";
}

void Game::End() {
    std::cout << "Game Run Time: " << timeManager->GetTimeSinceStartup() << '\n';
    std::cout << "Average FPS: " << timeManager->GetFrameCount() / timeManager->GetTimeSinceStartup() << '\n';
    std::cout << "Average Uncapped FPS: " << timeManager->GetNotFFRFrameCount() / timeManager->GetTimeSinceStartup()
              << '\n';
    std::cout << "Total Frames: " << timeManager->GetFrameCount() << '\n';

    VG::Log("Game Over!");
}
