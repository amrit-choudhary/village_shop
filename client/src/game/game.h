/**
 * Main Game class.
 * Derive this class for your own game.
 */

#pragma once

#include <cstdint>

#include "../anim/animation_system.h"
#include "../input/input_manager.h"
#include "../net/connection.h"
#include "../scene/scene.h"
#include "../scene/scene_ui.h"
#include "src/datastructure/ring_buffer.h"
#include "src/logging.h"
#include "src/math/math.h"
#include "src/physics/physics_system.h"
#include "src/random/random_engine.h"
#include "src/time/time_manager.h"
#include "villager.h"

namespace ME {

class Game {
   public:
    Game();
    ~Game();

    // Game control functions.

    // Init game
    virtual void Init(ME::Time::TimeManager *currentTimeManager);

    // Update game.
    virtual void Update(double deltaTime);

    // End game.
    virtual void End();

    void SetInputManagerRef(ME::Input::InputManager *ptrInput);
    void SetConnectionRef(ME::Connection *ptrConnection);
    void SetPhysicsSystemRef(ME::PhysicsSystem *ptrPhysicsSystem);
    void SetAnimationSystemRef(ME::AnimationSystem *ptrAnimationSystem);

    // This will be called from the PhysicsSystem when a collision is detected.
    // Remember to delete the result after use.
    virtual void CollisionCallback(ColliderAABB *a, ColliderAABB *b, CollisionResultAABB *result);

    ME::Scene *GetScene();
    ME::SceneUI *GetUIScene();

   protected:
    ME::Time::TimeManager *timeManager = nullptr;
    ME::Connection *connection = nullptr;
    ME::Input::InputManager *inputManager = nullptr;
    ME::Scene *scene = nullptr;
    ME::SceneUI *uiScene = nullptr;
    ME::PhysicsSystem *physicsSystem = nullptr;
    ME::AnimationSystem *animationSystem = nullptr;
};
}  // namespace ME
