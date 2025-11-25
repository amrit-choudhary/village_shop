/**
 * Collider class for physics interactions.
 */

#pragma once

#include <cstdint>

#include "physics_layer.h"

namespace ME {

/**
 * Collider class representing a physics collider.
 */
class Collider {
   public:
    Collider() : id(0), isEnabled(true), isStatic(true) {}
    Collider(uint32_t id, bool isEnabled, bool isStatic) : id(id), isEnabled(isEnabled), isStatic(isStatic) {}

    // Gets the ID of the collider.
    inline uint32_t GetID() const {
        return id;
    }

    bool isEnabled = true;
    bool isStatic = true;  // Indicates if the collider is static (not moving).

   private:
    uint32_t id = 0;  // Unique identifier for the collider.
};

}  // namespace ME
