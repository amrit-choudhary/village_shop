/**
 * Physics System.
 * This will handle the physics simulation, including collision detection and response.
 * It will also manage the physics scenes and their interactions.
 */

#pragma once

#include <stdint.h>

#include "physics_scene.h"

namespace ME {

class PhysicsSystem {
   public:
    // Initializes the physics system.
    void Init();

    // Updates the physics system with the given delta time.
    void Update(double deltaTime);

    // Ends the physics system, cleaning up resources if necessary.
    void End();

    // Set Scene for the physics system.
    void SetScene(ME::PhysicsScene* physicsScene);

    // Report collision between two colliders. Other systems can hook to this function to handle collisions.
    void ReportCollision(ME::ColliderAABB* colliderA, ME::ColliderAABB* colliderB);

   private:
    bool isInitialized = false;         // Flag to check if the system is initialized.
    ME::PhysicsScene* scene = nullptr;  // Current physics scene being managed.
};

}  // namespace ME
