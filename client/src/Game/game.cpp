#include <cmath>

#include "game.h"

Game::Game()
{
}

Game::~Game()
{
}

void Game::Init()
{
}

void Game::Update(double deltaTime)
{
    static double offset = 0;
    offset += deltaTime * 10;
    for (int y = 0; y < BUFFER_Y; ++y)
    {
        for (int x = 0; x < BUFFER_X; ++x)
        {
            double xOff = x + offset;
            double yOff = y + offset;

            double amplitude = 0.01f;
            double xS = std::abs(std::sin(xOff * amplitude));
            double yS = std::abs(std::sin(yOff * amplitude));

            double xR = xS * ASCII_COUNT;
            double yR = yS * ASCII_COUNT;

            uint8_t value = (xR + yR) / 2.0f;
            buffer1[x][y] = value;
        }
    }
}

void Game::End()
{
}
