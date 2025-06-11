/**
 * AABB Collider for 2D physics interactions.
 * This class represents an Axis-Aligned Bounding Box (AABB) collider,
 */

#pragma once

#include "../math/transform.h"
#include "collider.h"

namespace ME {

/**
 * Collision result for AABB colliders.
 * Will contain results such as penetration depth, normal vector, etc.
 */
class CollisionResultAABB {
   public:
    float penX = 0.0f;      // Penetration in X axis.
    float penY = 0.0f;      // Penetration in Y axis.
    float penDepth = 0.0f;  // Total penetration depth, calculated as the minimum of penX and penY.
    float normalX = 0.0f;   // Normal vector X component, indicating the direction of the collision.
    float normalY = 0.0f;   // Normal vector Y component, indicating the direction of the collision.
};

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

    // Constructor to initialize the AABB collider with a transform and scale multiplier.
    ColliderAABB(uint32_t id, bool isEnabled, bool isStatic, const ME::Transform& transform, float scaleMult = 1.0f);

    float minX = 0.0f;  // Minimum X coordinate of the AABB.
    float minY = 0.0f;  // Minimum Y coordinate of the AABB.
    float maxX = 0.0f;  // Maximum X coordinate of the AABB.
    float maxY = 0.0f;  // Maximum Y coordinate of the AABB.

    // Change the transform of the AABB collider based on a new transform.
    // scaleMult is a multiplier for the scale, this can make collider bigger or smaller than the sprite.
    void UpdateTransform(const ME::Transform& transform, float scaleMult = 1.0f);

    inline float GetCenterX() const {
        return (minX + maxX) / 2.0f;
    }

    inline float GetCenterY() const {
        return (minY + maxY) / 2.0f;
    }

    inline float GetWidth() const {
        return maxX - minX;
    }

    inline float GetHeight() const {
        return maxY - minY;
    }

    inline bool CheckCollision(const ColliderAABB& other) const {
        return !(other.minX > maxX || other.maxX < minX || other.minY > maxY || other.maxY < minY);
    }

    // Get more detailed collision result with another AABB collider. Only valid if CheckCollision returns true.
    // This returns a pointer to a CollisionResultAABB object, which contains the collision details.
    // This must be deleted by the caller to avoid memory leaks.
    // If there is no collision, this will return nullptr.
    // ME::Game should handle the deletion of this object after the collision callback.
    CollisionResultAABB* GetCollisionResult(const ColliderAABB& other) const;
};

}  // namespace ME
