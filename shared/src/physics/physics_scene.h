/**
 * Physics scene, containing all the physics objects and their interactions.
 */

#pragma once

#include "../../../client/src/scene/scene.h"
#include "collider_aabb.h"

namespace ME {

/**
 * PhysicsScene class representing a physics scene.
 * It contains list of static and dynamic colliders.
 */
class PhysicsScene {
   public:
    PhysicsScene();
    virtual ~PhysicsScene();

    // Initializes the physics scene with collider objects.
    virtual void Init();

    // Initializes the physics scene with a given scene.
    virtual void Init(const ME::Scene* scene);

    ME::ColliderAABB* staticColliders;
    ME::ColliderAABB* dynamicColliders;

    uint32_t staticColliderCount = 0;
    uint8_t dynamicColliderCount = 0;
};

}  // namespace ME
