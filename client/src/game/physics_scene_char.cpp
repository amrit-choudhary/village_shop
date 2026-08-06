#include "physics_scene_char.h"

namespace ME {

PhysicsSceneChar::PhysicsSceneChar() : PhysicsScene() {}

PhysicsSceneChar::~PhysicsSceneChar() {}

void PhysicsSceneChar::Init() {
    ME::PhysicsScene::Init();
}

void PhysicsSceneChar::Init(ColliderAABB* staticColliders, uint32_t staticColliderCount,
                             ColliderAABB* dynamicColliders, uint32_t dynamicColliderCount) {
    ME::PhysicsScene::Init(staticColliders, staticColliderCount, dynamicColliders, dynamicColliderCount);
}

}  // namespace ME
