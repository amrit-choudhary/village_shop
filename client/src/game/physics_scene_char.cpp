#include "physics_scene_char.h"

namespace ME {

PhysicsSceneChar::PhysicsSceneChar() : PhysicsScene() {}

PhysicsSceneChar::~PhysicsSceneChar() {}

void PhysicsSceneChar::Init() {
    ME::PhysicsScene::Init();
}

void PhysicsSceneChar::Init(ColliderAABB* inStaticColliders, uint32_t inStaticColliderCount,
                             ColliderAABB* inDynamicColliders, uint32_t inDynamicColliderCount) {
    ME::PhysicsScene::Init(inStaticColliders, inStaticColliderCount, inDynamicColliders, inDynamicColliderCount);
}

}  // namespace ME
