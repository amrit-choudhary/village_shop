#pragma once

/**
 * Falling Sand.
 */

#include <src/datastructure/grid.h>

#include "../scene/scene_falling_sand.h"
#include "game.h"

namespace ME {
class GameFallingSand : public Game {
   public:
    GameFallingSand();
    ~GameFallingSand();

    // Game control functions.

    // Init game
    virtual void Init(ME::Time::TimeManager *currentTimeManager) override;

    // Update game.
    virtual void Update(double deltaTime) override;

    // End game.
    virtual void End() override;

   private:
    SceneFallingSand *sandScene = nullptr;  // Scene for the game.
    PhysicsScene *physicsScene = nullptr;   // Physics scene for the game.
    const size_t gridWidth = 100;
    const size_t gridHeight = 100;
    const size_t gridCount = gridWidth * gridHeight;

    // Grids for the Game of Life.
    ME::Grid<uint8_t> *currentGen;
    ME::Grid<uint8_t> *nextGen;
    ME::Grid<uint8_t> *tempGrid;

    uint32_t frameCounter = 0;
    uint32_t updateIntervalFrames = 1;

    // Falling sand logic.
    void InitializeFallingSandLogic();
    void UpdateFallingSandLogic();
};

}  // namespace ME
