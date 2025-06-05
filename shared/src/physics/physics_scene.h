/**
 * Physics scene, containing all the physics objects and their interactions.
 */

#pragma once

#include "../../../client/src/scene/scene.h"
#include "collider_aabb.h"

namespace ME {

constexpr uint32_t MaxStaticColliders = 100'000;  // Maximum number of static colliders.
constexpr uint8_t MaxDynamicColliders = 32;       // Maximum number of dynamic colliders.

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

    ME::ColliderAABB* staticColliders = nullptr;   // Array of static colliders.
    ME::ColliderAABB* dynamicColliders = nullptr;  // Array of dynamic colliders.

    uint32_t staticColliderCount = 0;  // Count of static colliders.
    uint8_t dynamicColliderCount = 0;  // Count of dynamic colliders.
};

}  // namespace ME
