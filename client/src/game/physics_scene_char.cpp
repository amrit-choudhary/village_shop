#include "physics_scene_char.h"

namespace ME {

PhysicsSceneChar::PhysicsSceneChar() : PhysicsScene() {}

PhysicsSceneChar::~PhysicsSceneChar() {}

void PhysicsSceneChar::Init() {
    ME::PhysicsScene::Init();
}

void PhysicsSceneChar::Init(const ME::Scene* scene) {
    ME::PhysicsScene::Init(scene);
}

}  // namespace ME
