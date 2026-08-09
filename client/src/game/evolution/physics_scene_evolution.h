/**
 * Physics scene for the Evolution game.
 */

#pragma once

#include "shared/src/physics/collider_aabb.h"
#include "shared/src/physics/physics_scene.h"

namespace ME {

class PhysicsSceneEvolution : public ME::PhysicsScene {
   public:
    PhysicsSceneEvolution();
    virtual ~PhysicsSceneEvolution() override;

    // Initializes the physics scene with collider objects specific to Evolution.
    virtual void Init() override;

    // Initializes the physics scene with the given static/dynamic collider arrays specific to Evolution.
    virtual void Init(ColliderAABB* inStaticColliders, uint32_t inStaticColliderCount, ColliderAABB* inDynamicColliders,
                       uint32_t inDynamicColliderCount) override;
};

}  // namespace ME
