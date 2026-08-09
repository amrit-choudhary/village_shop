/**
 * Evolution Game.
 */

#pragma once

#include "client/src/game/evolution/physics_scene_evolution.h"
#include "client/src/game/evolution/scene_evolution.h"
#include "client/src/game/evolution/scene_ui_evolution.h"
#include "client/src/game/game.h"
#include "shared/src/datastructure/grid.h"

namespace ME {

class GameEvolution : public Game {
   public:
    GameEvolution();
    virtual ~GameEvolution();

    // Game control functions.

    // Init game
    virtual void Init(ME::Time::TimeManager* currentTimeManager) override;

    // Start game. This is called after all systems are initialized and before Update.
    virtual void Start() override;

    // Update game.
    virtual void Update(double deltaTime) override;

    // End game.
    virtual void End() override;

    virtual const char* GetDisplayName() const override;

   private:
    ME::SceneEvolution* evoScene = nullptr;                // Scene for the game.
    ME::SceneUIEvolution* evoUIScene = nullptr;            // UI scene for the game.
    ME::PhysicsSceneEvolution* evoPhysicsScene = nullptr;  // Physics scene for the game.

    // Game params.
    const size_t gridWidth = 10'000;
    const size_t gridHeight = 10'000;
    const size_t gridCount = gridWidth * gridHeight;
    ME::Grid<uint8_t>* terrain;
    const float cameraSpeed = 100.0f;
};

}  // namespace ME
