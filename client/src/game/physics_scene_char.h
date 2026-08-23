#pragma once

/**
 * Physics scene for the Character Test game.
 */

#include "shared/src/physics/collider_aabb.h"
#include "shared/src/physics/physics_scene.h"

namespace ME {

class PhysicsSceneChar : public ME::PhysicsScene {
   public:
    PhysicsSceneChar();
    virtual ~PhysicsSceneChar() override;

    // Initializes the physics scene with collider objects specific to Character Test.
    virtual void Init() override;

    // Initializes the physics scene with the given static/dynamic collider arrays specific to Character Test.
    virtual void Init(ME::Span<ColliderAABB> inStaticColliders, ME::Span<ColliderAABB> inDynamicColliders) override;
};

}  // namespace ME
