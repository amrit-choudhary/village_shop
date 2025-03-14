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
    for (int i = 0; i < BUFFER_X * BUFFER_Y; ++i)
    {
        healths[i].Update(deltaTime);
    }

    for (int i = 0; i < BUFFER_X * BUFFER_Y; ++i)
    {
        hungers[i].Update(deltaTime);
    }

    for (int i = 0; i < BUFFER_X * BUFFER_Y; ++i)
    {
        golds[i].Update(deltaTime);
    }

    static double offset = 0;
    offset += deltaTime * 10;
    for (int y = 0; y < BUFFER_Y; ++y)
    {
        for (int x = 0; x < BUFFER_X; ++x)
        {
            uint32_t index = y * BUFFER_Y + x;
            double val = (hungers[index].Hunger / 100.0f) * ASCII_COUNT_SHORT;

            buffer1[x][y] = ASCII_COUNT_SHORT - (uint8_t)val;
        }
    }
}

void Game::End()
{
}
