/**
 * Main Game class.
 */

#pragma once

#include <stdint.h>
#include <array>

#include "villager.h"
#include "../TimeManagement/TimeManager.h"
#include "src/logging.h"

constexpr uint8_t BUFFER_X = 80;
constexpr uint8_t BUFFER_X_2 = 40;
constexpr uint8_t BUFFER_Y = 40;
constexpr uint8_t BUFFER_Y_2 = 20;
constexpr uint8_t ASCII_COUNT = 69;
constexpr uint8_t ASCII_COUNT_SHORT = 10;

class Game
{
public:
    Game();
    ~Game();

    // Game control functions.

    // Init game
    void Init(VG::Time::TimeManager *currentTimeManager);

    // Update game.
    void Update(double deltaTime);

    // End game.
    void End();

    // Getters
    std::array<std::array<uint8_t, BUFFER_Y>, BUFFER_X> &GetBuffer()
    {
        return buffer1;
    }

private:
    VG::Time::TimeManager *timeManager;
    // Frame Buffer.
    std::array<std::array<uint8_t, BUFFER_Y>, BUFFER_X> buffer1;

    // Villager Health.
    VHealth healths[BUFFER_X * BUFFER_Y];
    // Villager Hunger.
    VHunger hungers[BUFFER_X * BUFFER_Y];
    // Villager Gold.
    VGold golds[BUFFER_X * BUFFER_Y];

    float x = 40;
    float y = 20;
};