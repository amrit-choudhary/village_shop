#include "collider_aabb.h"

ME::ColliderAABB::ColliderAABB(uint32_t id, bool isEnabled, bool isStatic, const ME::Transform& transform)
    : Collider(id, isEnabled, isStatic) {
    // Initialize the AABB based on the transform's position and scale.
    minX = transform.GetPosition().x - (transform.GetScale().x / 2.0f);
    minY = transform.GetPosition().y - (transform.GetScale().y / 2.0f);
    maxX = transform.GetPosition().x + (transform.GetScale().x / 2.0f);
    maxY = transform.GetPosition().y + (transform.GetScale().y / 2.0f);
}
