#include "physics_system.h"

#include "../../../client/src/game/game.h"

void ME::PhysicsSystem::Init() {}

void ME::PhysicsSystem::Update(double deltaTime) {
    if (scene == nullptr || !isInitialized) {
        return;
    }

    for (int i = 0; i < scene->staticColliderCount; ++i) {
        ColliderAABB* staticCollider = &scene->staticColliders[i];
        if (!staticCollider->isEnabled || !staticCollider->isStatic) {
            continue;
        }

        for (int j = 0; j < scene->dynamicColliderCount; ++j) {
            ColliderAABB* dynamicCollider = &scene->dynamicColliders[j];
            if (!dynamicCollider->isEnabled || dynamicCollider->isStatic) {
                continue;
            }
            if (staticCollider->CheckCollision(*dynamicCollider)) {
                ReportCollision(staticCollider, dynamicCollider);
            }
        }
    }
}

void ME::PhysicsSystem::End() {}

void ME::PhysicsSystem::SetScene(ME::PhysicsScene* physicsScene) {
    scene = physicsScene;  // Set the current physics scene.
    isInitialized = true;  // Mark the system as initialized.
}

void ME::PhysicsSystem::SetGame(ME::Game* game) {
    this->game = game;
}

void ME::PhysicsSystem::ReportCollision(ColliderAABB* a, ColliderAABB* b) {
    if (game != nullptr) {
        game->CollisionCallback(a, b);
    } else {
        __builtin_printf("Collision detected between colliders %u and %u\n", a->GetID(), b->GetID());
    }
}
