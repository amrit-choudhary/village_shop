/**
 * Physics scene for the Breakout game.
 */

#pragma once

#include "../scene/scene_breakout.h"
#include "src/physics/collider_aabb.h"
#include "src/physics/physics_scene.h"

namespace ME {

class PhysicsSceneBreakout : public ME::PhysicsScene {
   public:
    PhysicsSceneBreakout();
    ~PhysicsSceneBreakout() override;

    // Initializes the physics scene with collider objects specific to Breakout.
    void Init() override;

    // Initializes the physics scene with a given scene specific to Breakout.
    void Init(const ME::Scene* scene) override;
};

}  // namespace ME
