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
    virtual void Init(ME::Span<ColliderAABB> inStaticColliders, ME::Span<ColliderAABB> inDynamicColliders) override;
};

}  // namespace ME
