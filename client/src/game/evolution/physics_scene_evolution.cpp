#include "client/src/game/evolution/physics_scene_evolution.h"

namespace ME {

PhysicsSceneEvolution::PhysicsSceneEvolution() : PhysicsScene() {}

PhysicsSceneEvolution::~PhysicsSceneEvolution() {}

void PhysicsSceneEvolution::Init() {
    ME::PhysicsScene::Init();
}

void PhysicsSceneEvolution::Init(ColliderAABB* inStaticColliders, uint32_t inStaticColliderCount,
                                  ColliderAABB* inDynamicColliders, uint32_t inDynamicColliderCount) {
    ME::PhysicsScene::Init(inStaticColliders, inStaticColliderCount, inDynamicColliders, inDynamicColliderCount);
}

}  // namespace ME
