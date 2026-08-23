#include "physics_scene_breakout.h"

ME::PhysicsSceneBreakout::PhysicsSceneBreakout() : ME::PhysicsScene() {}

ME::PhysicsSceneBreakout::~PhysicsSceneBreakout() {}

void ME::PhysicsSceneBreakout::Init() {
    ME::PhysicsScene::Init();
}

void ME::PhysicsSceneBreakout::Init(ME::Span<ColliderAABB> inStaticColliders, ME::Span<ColliderAABB> inDynamicColliders) {
    ME::PhysicsScene::Init(inStaticColliders, inDynamicColliders);
}
