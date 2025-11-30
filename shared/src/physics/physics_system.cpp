#include "physics_system.h"

#include "../../../client/src/game/game.h"

void ME::PhysicsSystem::Init() {
    // Init the category indices and count for collision optimization.
    categoryIndices = new uint32_t*[ME::Constants::MaxPhysicsLayerCount];
    for (size_t i = 0; i < ME::Constants::MaxPhysicsLayerCount; ++i) {
        categoryIndices[i] = new uint32_t[ME::Constants::MaxDynamicColliderCount];
    }

    categoryCounts = new uint32_t[ME::Constants::MaxPhysicsLayerCount];
    for (size_t i = 0; i < ME::Constants::MaxPhysicsLayerCount; ++i) {
        categoryCounts[i] = 0;
    }
}

void ME::PhysicsSystem::Update(double deltaTime) {
    if (scene == nullptr || !isInitialized) {
        return;
    }

    // for (int i = 0; i < scene->staticColliderCount; ++i) {
    //     ColliderAABB* staticCollider = &scene->staticColliders[i];
    //     if (!staticCollider->isEnabled || !staticCollider->isStatic) {
    //         continue;
    //     }

    //     for (int j = 0; j < scene->dynamicColliderCount; ++j) {
    //         ColliderAABB* dynamicCollider = &scene->dynamicColliders[j];
    //         if (!dynamicCollider->isEnabled || dynamicCollider->isStatic) {
    //             continue;
    //         }
    //         CollisionResultAABB* result = dynamicCollider->GetCollisionResult(*staticCollider);
    //         if (result == nullptr) {
    //             continue;  // No collision detected.
    //         } else {
    //             ReportCollision(dynamicCollider, staticCollider, result);
    //         }
    //     }
    // }

    /**
     * Basic NxN collision detection between dynamic colliders only.
     * This is optimized by categorizing colliders based on their physics layers at startup.
     * Only colliders that belong to layers that can interact with each other are checked for collisions.
     * Steps:
     * 1. For each category, get the list of dynamic colliders in that category.
     * 2. For each collider in that category, check against colliders in other categories
     *   that are allowed to collide with it based on their masks.
     */
    for (uint8_t categoryIndex = 0; categoryIndex < ME::Constants::MaxPhysicsLayerCount; ++categoryIndex) {
        // Colliders in this category.
        uint32_t count = categoryCounts[categoryIndex];

        for (uint32_t i = 0; i < count; ++i) {
            uint32_t dynamicColliderIndexA = categoryIndices[categoryIndex][i];
            ColliderAABB* dynamicColliderA = &scene->dynamicColliders[dynamicColliderIndexA];

            if (!dynamicColliderA->isEnabled) {
                continue;
            }

            // Start checking from next category to avoid double checking.
            for (uint8_t otherCategoryIndex = (categoryIndex + 1);
                 otherCategoryIndex < ME::Constants::MaxPhysicsLayerCount; ++otherCategoryIndex) {
                if (categoryIndex == otherCategoryIndex) {
                    continue;  // Skip same category to avoid double checking.
                }

                PhysicsLayer otherCategoryLayer = ME::Physics::GetPhysicsLayerFromIndex(otherCategoryIndex);
                if (ME::Physics::HasLayer(dynamicColliderA->mask, otherCategoryLayer) == false) {
                    continue;  // No collision allowed between these layers.
                }

                uint32_t otherCount = categoryCounts[otherCategoryIndex];
                for (uint32_t j = 0; j < otherCount; ++j) {
                    uint32_t dynamicColliderIndexB = categoryIndices[otherCategoryIndex][j];
                    ColliderAABB* dynamicColliderB = &scene->dynamicColliders[dynamicColliderIndexB];

                    if (!dynamicColliderB->isEnabled) {
                        continue;
                    }

                    if (!ME::Physics::IsCollisionAllowed(dynamicColliderA->category, dynamicColliderA->mask,
                                                         dynamicColliderB->category, dynamicColliderB->mask)) {
                        continue;
                    }

                    CollisionResultAABB* result = dynamicColliderA->GetCollisionResult(*dynamicColliderB);
                    if (result == nullptr) {
                        continue;  // No collision detected.
                    } else {
                        ReportCollision(dynamicColliderA, dynamicColliderB, result);
                    }
                }
            }
        }
    }
}

void ME::PhysicsSystem::End() {
    for (size_t i = 0; i < ME::Constants::MaxPhysicsLayerCount; ++i) {
        delete[] categoryIndices[i];
        categoryIndices[i] = nullptr;
    }
    delete[] categoryIndices;
    categoryIndices = nullptr;
    delete[] categoryCounts;
    categoryCounts = nullptr;

    isInitialized = false;
}

void ME::PhysicsSystem::SetScene(ME::PhysicsScene* physicsScene) {
    scene = physicsScene;  // Set the current physics scene.
    SetupCollisionCategories();
    isInitialized = true;  // Mark the system as initialized.
}

void ME::PhysicsSystem::SetGame(ME::Game* game) {
    this->game = game;
}

void ME::PhysicsSystem::ReportCollision(ColliderAABB* a, ColliderAABB* b, CollisionResultAABB* result) {
    if (game != nullptr) {
        game->CollisionCallback(a, b, result);
    } else {
        // __builtin_printf("Collision detected between colliders %u and %u\n", a->GetID(), b->GetID());
    }
}

void ME::PhysicsSystem::SetupCollisionCategories() {
    for (size_t i = 0; i < ME::Constants::MaxPhysicsLayerCount; ++i) {
        categoryCounts[i] = 0;
    }

    for (uint32_t i = 0; i < scene->dynamicColliderCount; ++i) {
        ColliderAABB* dynamicCollider = &scene->dynamicColliders[i];
        if (dynamicCollider->isStatic) {
            continue;
        }
        uint8_t layerIndex = ME::Physics::GetIndexFromPhysicsLayer(dynamicCollider->category);
        uint32_t count = categoryCounts[layerIndex];
        categoryIndices[layerIndex][count] = i;
        categoryCounts[layerIndex] = count + 1;
    }
}
