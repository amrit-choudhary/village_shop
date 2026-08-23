#include "client/src/game/evolution/physics_scene_evolution.h"

namespace ME {

PhysicsSceneEvolution::PhysicsSceneEvolution() : PhysicsScene() {}

PhysicsSceneEvolution::~PhysicsSceneEvolution() {}

void PhysicsSceneEvolution::Init() {
    ME::PhysicsScene::Init();
}

void PhysicsSceneEvolution::Init(ME::Span<ColliderAABB> inStaticColliders, ME::Span<ColliderAABB> inDynamicColliders) {
    ME::PhysicsScene::Init(inStaticColliders, inDynamicColliders);
}

}  // namespace ME
