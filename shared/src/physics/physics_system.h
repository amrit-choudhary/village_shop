/**
 * Physics System.
 * This will handle the physics simulation, including collision detection and response.
 * It will also manage the physics scenes and their interactions.
 */

#pragma once

#include <stdint.h>

namespace ME {

class PhysicsSystem {
   public:
    // Initializes the physics system.
    void Init();

    // Updates the physics system with the given delta time.
    void Update(double deltaTime);

    // Ends the physics system, cleaning up resources if necessary.
    void End();

   private:
};

}  // namespace ME
