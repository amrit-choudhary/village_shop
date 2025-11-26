#include "audio_system.h"

#include "../scene/scene.h"

void ME::AudioSystem::Init() {}

void ME::AudioSystem::Update(double deltaTime) {
    if (scene == nullptr || !isInitialized) {
        return;
    }

    // Audio system update logic would go here.
}

void ME::AudioSystem::End() {}

void ME::AudioSystem::SetScene(Scene* scene) {
    this->scene = scene;   // Set the current scene.
    isInitialized = true;  // Mark the system as initialized.

    // Load audio resources related to the scene here.
}

void ME::AudioSystem::PlayMusic(uint8_t musicId, bool bLoop) {}

void ME::AudioSystem::StopMusic() {}

void ME::AudioSystem::PlaySound(uint8_t soundId, bool bLoop) {}

void ME::AudioSystem::StopSound(uint8_t soundId, bool bLoop) {}

void ME::AudioSystem::SetMasterVolume(float volume) {}

float ME::AudioSystem::GetMasterVolume() {
    return 0.0f;
}

void ME::AudioSystem::ToggleMuteAllSounds(bool bMute) {}
