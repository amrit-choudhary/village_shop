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
    const size_t maxNPCCount = 64;
    ME::Transform* playerTransform = nullptr;
    uint32_t score = 0;
    ME::Vec2* bulletDirs = nullptr;
    const size_t maxBulletCount = 9000;
    const float bulletSpeed = 80.0f;
    const size_t fireRate = 8;
    const size_t burstCount = 4;
    size_t burstCounter = 0;
    size_t frameCounter = 0;
    ME::Vec3 bulletParkPos{9000.0f, 9000.0f, 0.0f};
    ME::Vec3 bulletStartPos{0.0f, 0.0f, 0.0f};
    float bulletStartDirAngle = 0.0f;
    ME::Random rndBullet{"bullet", true};
    size_t cycleCounter = 0;
};

}  // namespace ME
