#pragma once

/**
 * Character Animation Test Game.
 */

#include "../scene/scene_character_test.h"
#include "game.h"

namespace ME {

class GameCharacterTest : public Game {
   public:
    GameCharacterTest();
    ~GameCharacterTest();

    // Game control functions.

    // Init game
    virtual void Init(ME::Time::TimeManager* currentTimeManager) override;

    // Update game.
    virtual void Update(double deltaTime) override;

    // End game.
    virtual void End() override;

   private:
    ME::SceneCharacterTest* charScene = nullptr;  // Scene for the game.
    ME::PhysicsScene* physicsScene = nullptr;     // Physics scene for the game.

    const float enemyBaseSpeed = 10.0f;
    const float enemySpeedVariance = 5.0f;
    const float outThrowDistance = 70.0f;
    const float minDistaneToPlayerSqr = 10.0f;
    const size_t maxNPCCount = 200;
    ME::Transform* playerTransform = nullptr;
};

}  // namespace ME
