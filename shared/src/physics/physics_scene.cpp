#include "physics_scene.h"

ME::PhysicsScene::PhysicsScene() {}

ME::PhysicsScene::~PhysicsScene() {
    // Clean up the static and dynamic colliders.
    delete[] staticColliders;
    delete[] dynamicColliders;
}

void ME::PhysicsScene::Init() {
    // Initialize the static and dynamic colliders.
    staticColliders = new ME::ColliderAABB[MaxStaticColliders];
    dynamicColliders = new ME::ColliderAABB[MaxDynamicColliders];
}
