#include "physics_scene.h"

ME::PhysicsScene::PhysicsScene() {}

ME::PhysicsScene::~PhysicsScene() {
    delete[] mapStaticIdToIndex;
    delete[] mapDynamicIdToIndex;
}

void ME::PhysicsScene::Init() {
    mapStaticIdToIndex = new uint32_t[Constants::MaxStaticColliderCount];
    mapDynamicIdToIndex = new uint32_t[Constants::MaxDynamicColliderCount];
}

void ME::PhysicsScene::Init(const ME::Scene* scene) {
    Init();

    staticColliders = scene->staticColliders;
    dynamicColliders = scene->dynamicColliders;

    staticColliderCount = scene->staticColliderCount;
    dynamicColliderCount = scene->dynamicColliderCount;

    // TODO FIX sparse array.

    // for (uint32_t i = 0; i < staticColliderCount; ++i) {
    //     mapStaticIdToIndex[staticColliders[i].GetID()] = i;
    // }
    // for (uint8_t i = 0; i < dynamicColliderCount; ++i) {
    //     mapDynamicIdToIndex[dynamicColliders[i].GetID()] = i;
    // }
}

ME::Collider* ME::PhysicsScene::GetStaticColliderById(uint32_t id) const {
    if (id < staticColliderCount) {
        return &staticColliders[mapStaticIdToIndex[id]];
    } else {
        return nullptr;
    }
}

ME::Collider* ME::PhysicsScene::GetDynamicColliderById(uint32_t id) const {
    if (id < dynamicColliderCount) {
        return &dynamicColliders[mapDynamicIdToIndex[id]];
    } else {
        return nullptr;
    }
}
