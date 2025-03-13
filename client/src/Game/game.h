/**
 * Main Game class.
 */

#pragma once

#include <stdint.h>
#include <array>

constexpr uint8_t BUFFER_X = 80;
constexpr uint8_t BUFFER_X_2 = 40;
constexpr uint8_t BUFFER_Y = 40;
constexpr uint8_t BUFFER_Y_2 = 20;
constexpr uint8_t ASCII_COUNT = 70;

class Game
{
public:
    Game();
    ~Game();

    // Game control functions.

    // Init game
    void Init();

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
    // Frame Buffer.
    std::array<std::array<uint8_t, BUFFER_Y>, BUFFER_X> buffer1;
};