#include "physics_scene_breakout.h"

ME::PhysicsSceneBreakout::PhysicsSceneBreakout() : ME::PhysicsScene() {}

ME::PhysicsSceneBreakout::~PhysicsSceneBreakout() {}

void ME::PhysicsSceneBreakout::Init() {
    ME::PhysicsScene::Init();
}

void ME::PhysicsSceneBreakout::Init(ColliderAABB* inStaticColliders, uint32_t inStaticColliderCount,
                                     ColliderAABB* inDynamicColliders, uint32_t inDynamicColliderCount) {
    ME::PhysicsScene::Init(inStaticColliders, inStaticColliderCount, inDynamicColliders, inDynamicColliderCount);
}
