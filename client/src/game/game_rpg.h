#pragma once

/**
 * RPG Game Demo.
 */

#include "../scene/scene_rpg.h"
#include "game.h"
#include "src/datastructure/grid.h"

namespace ME {

class GameRPG : public Game {
   public:
    GameRPG();
    ~GameRPG();

    // Game control functions.

    // Init game
    virtual void Init(ME::Time::TimeManager* currentTimeManager) override;

    // Update game.
    virtual void Update(double deltaTime) override;

    // End game.
    virtual void End() override;

   private:
    ME::SceneRPG* rpgScene = nullptr;          // Scene for the game.
    ME::PhysicsScene* physicsScene = nullptr;  // Physics scene for the game.
    const size_t gridWidth = 48;
    const size_t gridHeight = 100;
    const size_t gridCount = gridWidth * gridHeight;

    uint32_t frameCounter = 0;
    uint32_t updateIntervalFrames = 1;
};

}  // namespace ME
