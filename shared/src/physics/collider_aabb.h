/**
 * AABB Collider for 2D physics interactions.
 * This class represents an Axis-Aligned Bounding Box (AABB) collider,
 */

#pragma once

#include "../math/transform.h"
#include "collider.h"

namespace ME {

/**
 * AABB Collider class representing a 2D Axis-Aligned Bounding Box collider.
 * It contains the minimum and maximum points of the AABB.
 */
class ColliderAABB : public Collider {
   public:
    // Default constructor
    ColliderAABB() : Collider(0, true, true), minX(0.0f), minY(0.0f), maxX(0.0f), maxY(0.0f) {}
    // Constructor to initialize the AABB collider with a given ID, min and max points.
    ColliderAABB(uint32_t id, bool isEnabled, bool isStatic, float minX, float minY, float maxX, float maxY)
        : Collider(id, isEnabled, isStatic), minX(minX), minY(minY), maxX(maxX), maxY(maxY) {}

    ColliderAABB(uint32_t id, bool isEnabled, bool isStatic, const ME::Transform& transform);

    float minX = 0.0f;  // Minimum X coordinate of the AABB.
    float minY = 0.0f;  // Minimum Y coordinate of the AABB.
    float maxX = 0.0f;  // Maximum X coordinate of the AABB.
    float maxY = 0.0f;  // Maximum Y coordinate of the AABB.
};

}  // namespace ME
