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
            uint8_t value = 0;
            uint8_t xi = (x + (uint32_t)offset) % BUFFER_X;
            uint8_t yi = (y + (uint32_t)offset) % BUFFER_Y;
            float xf = (std::abs(xi - BUFFER_X_2) / (float)BUFFER_X_2) * ASCII_COUNT;
            float yf = (std::abs(yi - BUFFER_Y_2) / (float)BUFFER_Y_2) * ASCII_COUNT;
            value = 70 - std::min(xf, yf);

            buffer1[x][y] = value;
        }
    }
}

void Game::End()
{
}
