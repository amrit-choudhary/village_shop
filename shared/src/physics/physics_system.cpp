#include "physics_system.h"

void ME::PhysicsSystem::Init() {}

void ME::PhysicsSystem::Update(double deltaTime) {}

void ME::PhysicsSystem::End() {}

void ME::PhysicsSystem::SetScene(ME::PhysicsScene* physicsScene) {
    scene = physicsScene;  // Set the current physics scene.
    isInitialized = true;  // Mark the system as initialized.
}

void ME::PhysicsSystem::ReportCollision(ME::ColliderAABB* colliderA, ME::ColliderAABB* colliderB) {
    __builtin_printf("Collision detected between ColliderA (ID: %u) and ColliderB (ID: %u)\n", colliderA->GetID(),
                     colliderB->GetID());
}
