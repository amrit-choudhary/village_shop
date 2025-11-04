#pragma once

/**
 * Game of Life.
 */

#include <src/datastructure/grid.h>

#include "../scene/scene_game_of_life.h"
#include "game.h"

namespace ME {

class GameOfLife : public Game {
   public:
    GameOfLife();
    ~GameOfLife();

    // Game control functions.

    // Init game
    virtual void Init(ME::Time::TimeManager *currentTimeManager) override;

    // Update game.
    virtual void Update(double deltaTime) override;

    // End game.
    virtual void End() override;

   private:
    SceneGameOfLife *golScene = nullptr;   // Scene for the game.
    PhysicsScene *physicsScene = nullptr;  // Physics scene for the game.
    const size_t gridWidth = 100;
    const size_t gridHeight = 100;
    const size_t gridCount = gridWidth * gridHeight;

    // Grids for the Game of Life.
    ME::Grid<uint8_t> *currentGen;
    ME::Grid<uint8_t> *nextGen;
    ME::Grid<uint8_t> *tempGrid;

    uint32_t frameCounter = 0;
    uint32_t updateIntervalFrames = 2;

    // Implementation of Brian's Brain variation.
    bool bBriansBrainMode = false;
    uint8_t birthNumber = 2;

    // Game of life logic.
    void InitializeGameOfLifeLogic();
    void UpdateGameOfLifeLogic();
    void UpdateGameOfLifeLogicBriansBrain();
};

}  // namespace ME
