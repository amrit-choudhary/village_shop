/**
 * Physics scene for the Breakout game.
 */

#pragma once

#include "shared/src/physics/collider_aabb.h"
#include "shared/src/physics/physics_scene.h"

namespace ME {

class PhysicsSceneBreakout : public ME::PhysicsScene {
   public:
    PhysicsSceneBreakout();
    ~PhysicsSceneBreakout() override;

    // Initializes the physics scene with collider objects specific to Breakout.
    void Init() override;

    // Initializes the physics scene with the given static/dynamic collider arrays specific to Breakout.
    void Init(ColliderAABB* staticColliders, uint32_t staticColliderCount, ColliderAABB* dynamicColliders,
              uint32_t dynamicColliderCount) override;
};

}  // namespace ME
