/**
 * Breakout Game
 */

#pragma once

#include "../scene/scene_breakout.h"
#include "game.h"
#include "physics_scene_breakout.h"
#include "src/physics/collider_aabb.h"

namespace ME {

class GameBreakout : public Game {
   public:
    GameBreakout();
    ~GameBreakout();

    // Game control functions.

    // Init game
    virtual void Init(ME::Time::TimeManager *currentTimeManager) override;

    // Update game.
    virtual void Update(double deltaTime) override;

    // End game.
    virtual void End() override;

    // This will be called from the PhysicsSystem when a collision is detected.
    void CollisionCallback(ME::ColliderAABB *a, ME::ColliderAABB *b) override;

   private:
    SceneBreakout *brkScene = nullptr;                       // Scene for the game.
    PhysicsScene *physicsScene = nullptr;                    // Physics scene for the game.
    Transform *ballTransform = nullptr;                      // Transform for the ball.
    SpriteRendererInstanceData *ballInstanceData = nullptr;  // Instance data for the ball.
    Vec2 ballVelocity{1.0f, 1.0f};                           // Initial velocity of the ball.
    ColliderAABB *ballCollider = nullptr;                    // Collider for the ball.
    uint32_t ballIndex = 0;
    uint32_t score = 0;

    bool IsDestructible(uint32_t index) const;
};

}  // namespace ME
