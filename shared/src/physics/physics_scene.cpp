#include "physics_scene.h"

ME::PhysicsScene::PhysicsScene() {}

ME::PhysicsScene::~PhysicsScene() {}

void ME::PhysicsScene::Init() {}

void ME::PhysicsScene::Init(const ME::Scene* scene) {
    staticColliders = scene->staticColliders;
    dynamicColliders = scene->dynamicColliders;

    staticColliderCount = scene->staticColliderCount;
    dynamicColliderCount = scene->dynamicColliderCount;
}
