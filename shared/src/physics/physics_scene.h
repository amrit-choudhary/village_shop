/**
 * Physics scene, containing all the physics objects and their interactions.
 */

#pragma once

#include <cstdint>

#include "../../../client/src/scene/scene.h"
#include "collider_aabb.h"
#include "src/misc/game_constants.h"

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

    ColliderAABB* staticColliders;
    ColliderAABB* dynamicColliders;

    uint32_t staticColliderCount = 0;
    uint8_t dynamicColliderCount = 0;

    Collider* GetStaticColliderById(uint32_t id) const;
    Collider* GetDynamicColliderById(uint32_t id) const;

   private:
    uint32_t* mapStaticIdToIndex;
    uint32_t* mapDynamicIdToIndex;
};

}  // namespace ME
