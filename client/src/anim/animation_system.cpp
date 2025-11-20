#include "animation_system.h"

#include "../scene/scene.h"

void ME::AnimationSystem::Init() {}

void ME::AnimationSystem::Update(double deltaTime) {
    if (scene == nullptr || !isInitialized) {
        return;
    }

    // Update all sprite animators in the scene.
    for (uint16_t i = 0; i < scene->spriteRendererCount; ++i) {
        ME::SpriteRenderer* spriteRenderer = scene->spriteRenderers[i];
        if (spriteRenderer != nullptr && spriteRenderer->animator != nullptr) {
            spriteRenderer->animator->AnimationUpdate();
        }
    }

    // Update all instanced sprite animators in the scene.
    for (uint16_t i = 0; i < scene->instancedSpriteRendererCount0; ++i) {
        ME::SpriteRenderer* spriteRenderer = scene->instancedSpriteRenderers0[i];
        if (spriteRenderer != nullptr && spriteRenderer->animator != nullptr) {
            spriteRenderer->animator->AnimationUpdate();
        }
    }
}

void ME::AnimationSystem::End() {}

void ME::AnimationSystem::SetScene(ME::Scene* scene) {
    this->scene = scene;   // Set the current scene.
    isInitialized = true;  // Mark the system as initialized.
}
