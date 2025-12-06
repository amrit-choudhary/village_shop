#pragma once

/**
 * Character Animation Test Game.
 */

#include "../scene/scene_character_test.h"
#include "game.h"
#include "wave_data.h"

namespace ME {

class GameCharacterTest : public Game {
   public:
    GameCharacterTest();
    virtual ~GameCharacterTest();

    // Game control functions.

    // Init game
    virtual void Init(ME::Time::TimeManager* currentTimeManager) override;

    // Start game. This is called after all systems are initialized and before Update.
    virtual void Start() override;

    // Update game.
    virtual void Update(double deltaTime) override;

    // End game.
    virtual void End() override;

    // This will be called from the PhysicsSystem when a collision is detected.
    virtual void CollisionCallback(ColliderAABB* a, ColliderAABB* b, CollisionResultAABB* result) override;

   private:
    ME::SceneCharacterTest* charScene = nullptr;  // Scene for the game.
    ME::PhysicsScene* physicsScene = nullptr;     // Physics scene for the game.

    const float enemyBaseSpeed = 10.0f;
    const float enemySpeedVariance = 5.0f;
    const float outThrowDistance = 50.0f;
    const float minDistaneToPlayerSqr = 10.0f;
    const size_t maxNPCCount = 256;
    ME::Transform* playerTransform = nullptr;
    uint32_t score = 0;
    uint32_t health = 100;
    ME::Vec2* bulletDirs = nullptr;
    const size_t maxBulletCount = 100;
    const float bulletSpeed = 80.0f;
    const size_t fireRate = 8;
    const size_t burstCount = 4;
    size_t burstCounter = 0;
    size_t frameCounter = 0;
    ME::Vec3 npcParkPos{9000.0f, 9000.0f, 0.0f};
    ME::Vec3 bulletParkPos{-9000.0f, -9000.0f, 0.0f};
    ME::Vec3 bulletStartPos{0.0f, 0.0f, 0.0f};
    float bulletStartDirAngle = 0.0f;
    ME::Random rndBullet{"bullet", true};
    size_t cycleCounter = 0;

    // Wave data.
    WaveData* waveData = nullptr;
    SingleWave* currentWave = nullptr;
    uint32_t currentWaveIndex = 0;
    uint32_t enemiesRemainingInWave = 0;
    Enemy* enemies = nullptr;
    uint32_t spawnSpeedCounter = 0;
    uint32_t spawnCounter = 0;

    void SpawnNextEnemy();

    ME::SpriteAnimClip* clipBase = nullptr;
};

}  // namespace ME
