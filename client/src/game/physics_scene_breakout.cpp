#include "physics_scene_breakout.h"

ME::PhysicsSceneBreakout::PhysicsSceneBreakout() : ME::PhysicsScene() {}

ME::PhysicsSceneBreakout::~PhysicsSceneBreakout() {}

void ME::PhysicsSceneBreakout::Init() {
    staticColliders = new ME::ColliderAABB[MaxStaticColliders];
    dynamicColliders = new ME::ColliderAABB[MaxDynamicColliders];

    staticColliderCount = 0;
    dynamicColliderCount = 0;
}

void ME::PhysicsSceneBreakout::Init(const ME::Scene* scene) {
    Init();
    uint32_t count = 0;
    for (int i = 0; i < scene->instancedSpriteTransformCount; ++i) {
        // Only add colliders for the bricks (atlasIndex 587).
        if (scene->spriteInstanceData[i]->atlasIndex == 587 || scene->spriteInstanceData[i]->atlasIndex == 253) {
            staticColliders[count] = ME::ColliderAABB(count, true, true, (*scene->instancedSpriteTransforms[i]));
            ++count;
        }
    }
    staticColliderCount = count;

    uint16_t ballIndex = dynamic_cast<const ME::SceneBreakout*>(scene)->ballIndex;
    dynamicColliders[0] = ME::ColliderAABB(0, true, false, (*scene->instancedSpriteTransforms[ballIndex]));

    dynamicColliderCount = 1;
}
