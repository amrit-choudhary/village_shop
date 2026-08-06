#include "physics_scene_breakout.h"

ME::PhysicsSceneBreakout::PhysicsSceneBreakout() : ME::PhysicsScene() {}

ME::PhysicsSceneBreakout::~PhysicsSceneBreakout() {}

void ME::PhysicsSceneBreakout::Init() {
    ME::PhysicsScene::Init();
}

void ME::PhysicsSceneBreakout::Init(ColliderAABB* staticColliders, uint32_t staticColliderCount,
                                     ColliderAABB* dynamicColliders, uint32_t dynamicColliderCount) {
    ME::PhysicsScene::Init(staticColliders, staticColliderCount, dynamicColliders, dynamicColliderCount);
}
