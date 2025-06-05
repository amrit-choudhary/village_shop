#include "collider_aabb.h"

ME::ColliderAABB::ColliderAABB(uint32_t id, bool isEnabled, bool isStatic, const ME::Transform& transform)
    : Collider(id, isEnabled, isStatic) {
    // Initialize the AABB based on the transform's position and scale.
    ME::Vec3 pos = transform.GetPosition();
    ME::Vec3 scale = transform.GetScale();
    minX = pos.x - (scale.x / 2.0f);
    minY = pos.y - (scale.y / 2.0f);
    maxX = pos.x + (scale.x / 2.0f);
    maxY = pos.y + (scale.y / 2.0f);
}

void ME::ColliderAABB::UpdateTransform(const ME::Transform& transform) {
    {
        ME::Vec3 pos = transform.GetPosition();
        ME::Vec3 scale = transform.GetScale();
        minX = pos.x - (scale.x / 2.0f);
        minY = pos.y - (scale.y / 2.0f);
        maxX = pos.x + (scale.x / 2.0f);
        maxY = pos.y + (scale.y / 2.0f);
    }
}
