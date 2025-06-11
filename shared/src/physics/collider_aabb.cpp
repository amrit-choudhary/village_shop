#include "collider_aabb.h"

ME::ColliderAABB::ColliderAABB(uint32_t id, bool isEnabled, bool isStatic, const ME::Transform& transform,
                               float scaleMult)
    : Collider(id, isEnabled, isStatic) {
    Vec3 pos = transform.GetPosition();
    Vec3 scale = transform.GetScale() * scaleMult;
    minX = pos.x - (scale.x / 2.0f);
    minY = pos.y - (scale.y / 2.0f);
    maxX = pos.x + (scale.x / 2.0f);
    maxY = pos.y + (scale.y / 2.0f);
}

void ME::ColliderAABB::UpdateTransform(const ME::Transform& transform, float scaleMult) {
    {
        Vec3 pos = transform.GetPosition();
        Vec3 scale = transform.GetScale() * scaleMult;
        minX = pos.x - (scale.x / 2.0f);
        minY = pos.y - (scale.y / 2.0f);
        maxX = pos.x + (scale.x / 2.0f);
        maxY = pos.y + (scale.y / 2.0f);
    }
}

ME::CollisionResultAABB* ME::ColliderAABB::GetCollisionResult(const ColliderAABB& other) const {
    if (!CheckCollision(other)) return nullptr;

    CollisionResultAABB* result = new CollisionResultAABB();

    return result;
}
