/**
 * Main Game class.
 */

#pragma once

#include <stdint.h>

#include <array>

#include "../TimeManagement/TimeManager.h"
#include "../datastructure/ring_buffer.h"
#include "../math/math.h"
#include "../random/random_engine.h"
#include "src/logging.h"
#include "villager.h"

constexpr uint8_t BUFFER_X = 80;
constexpr uint8_t BUFFER_X_2 = 40;
constexpr uint8_t BUFFER_Y = 40;
constexpr uint8_t BUFFER_Y_2 = 20;
constexpr uint8_t ASCII_COUNT = 69;
constexpr uint8_t ASCII_COUNT_SHORT = 10;

class Game {
   public:
    Game();
    ~Game();

    // Game control functions.

    // Init game
    void Init(ME::Time::TimeManager *currentTimeManager);

    // Update game.
    void Update(double deltaTime);

    // End game.
    void End();

    // Getters
    std::array<std::array<uint8_t, BUFFER_Y>, BUFFER_X> &GetBuffer() { return buffer1; }

   private:
    ME::Time::TimeManager *timeManager;
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

    ME::RingBuffer<uint8_t> *ringBuffer;
    uint8_t index;

    ME::Random rnd{ME::Random(12345)};

    uint8_t lut[10] = {0, 0, 0, 1, 1, 1, 2, 2, 2, 3};
    ME::RandomWt rndWt{12345, lut};
};
