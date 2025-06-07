/**
 * Physics System.
 * This will handle the physics simulation, including collision detection and response.
 * It will also manage the physics scenes and their interactions.
 */

#pragma once

#include <stdint.h>

#include "physics_scene.h"

namespace ME {

// Forward declarations.
class Game;

class PhysicsSystem {
   public:
    // Initializes the physics system.
    void Init();

    // Updates the physics system with the given delta time.
    void Update(double deltaTime);

    // Ends the physics system, cleaning up resources if necessary.
    void End();

    // Set Scene for the physics system.
    void SetScene(PhysicsScene* physicsScene);

    // Set the current game instance, if needed.
    void SetGame(Game* game);

    // Report collision between two colliders. Other systems can hook to this function to handle collisions.
    void ReportCollision(ColliderAABB* a, ColliderAABB* b);

   private:
    bool isInitialized = false;     // Flag to check if the system is initialized.
    PhysicsScene* scene = nullptr;  // Current physics scene being managed.
    Game* game = nullptr;           // Reference to the game instance, if needed.
};

}  // namespace ME
