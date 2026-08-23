#include "physics_scene_char.h"

namespace ME {

PhysicsSceneChar::PhysicsSceneChar() : PhysicsScene() {}

PhysicsSceneChar::~PhysicsSceneChar() {}

void PhysicsSceneChar::Init() {
    ME::PhysicsScene::Init();
}

void PhysicsSceneChar::Init(ME::Span<ColliderAABB> inStaticColliders, ME::Span<ColliderAABB> inDynamicColliders) {
    ME::PhysicsScene::Init(inStaticColliders, inDynamicColliders);
}

}  // namespace ME
