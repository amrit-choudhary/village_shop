/**
 * Main Game class.
 * Derive this class for your own game.
 */

#pragma once

#include <cstdint>

#include "client/src/anim/animation_system.h"
#include "client/src/audio/audio_system.h"
#include "client/src/input/input_manager.h"
#include "client/src/net/connection.h"
#include "client/src/scene/scene.h"
#include "client/src/scene/scene_ui.h"
#include "client/src/ui/ui_system.h"
#include "shared/src/datastructure/ring_buffer.h"
#include "logging/src/logging.h"
#include "shared/src/math/math.h"
#include "shared/src/physics/physics_system.h"
#include "shared/src/random/random_engine.h"
#include "shared/src/time/time_manager.h"
#include "villager.h"

namespace ME {

class Game : public ICollisionListener {
   public:
    Game();
    virtual ~Game();

    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
    Game(Game&&) = delete;
    Game& operator=(Game&&) = delete;

    // Game control functions.

    // Init game
    virtual void Init(ME::Time::TimeManager* currentTimeManager);

    // Start game. This is called after all systems are initialized and before Update.
    virtual void Start();

    // Update game. Called once per real frame with a variable delta time -
    // input/UI/render-facing logic that doesn't need a constant dt.
    virtual void Update(double deltaTime);

    // Fixed-timestep update. Called 0, 1, or several times per real frame with a constant
    // delta time (ME::Time::TimeManager::GetFixedDeltaTime()) - gameplay/physics logic that
    // needs to behave the same regardless of render frame rate.
    virtual void FixedUpdate(double fixedDeltaTime);

    // End game.
    virtual void End();

    void SetInputManagerRef(ME::Input::InputManager* ptrInput);
    void SetConnectionRef(ME::Connection* ptrConnection);
    void SetPhysicsSystemRef(ME::PhysicsSystem* ptrPhysicsSystem);
    void SetAnimationSystemRef(ME::AnimationSystem* ptrAnimationSystem);
    void SetAudioSystemRef(ME::AudioSystem* ptrAudioSystem);
    void SetUISystemRef(ME::UISystem* ptrUISystem);

    // This will be called from the PhysicsSystem when a collision is detected.
    // Remember to delete the result after use.
    void CollisionCallback(ColliderAABB* a, ColliderAABB* b, CollisionResultAABB* result) override;

    ME::Scene* GetScene();
    ME::SceneUI* GetUIScene();

    // Human-readable label for this game, shown on the debug overlay. Override to customize.
    virtual const char* GetDisplayName() const;

   protected:
    ME::Time::TimeManager* timeManager = nullptr;
    ME::Connection* connection = nullptr;
    ME::Input::InputManager* inputManager = nullptr;
    ME::Scene* scene = nullptr;
    ME::SceneUI* uiScene = nullptr;
    ME::PhysicsSystem* physicsSystem = nullptr;
    ME::AnimationSystem* animationSystem = nullptr;
    ME::AudioSystem* audioSystem = nullptr;
    ME::UISystem* uiSystem = nullptr;
};
}  // namespace ME
