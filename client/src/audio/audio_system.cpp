#include "audio_system.h"

#include "../scene/scene.h"
#include "audio_impl_miniaudio.h"

void ME::AudioSystem::Init() {
    isInitialized = false;
    audioImpl = new AudioImplMiniaudio();
    audioImpl->Init(this);
    isInitialized = true;
}

void ME::AudioSystem::Update(double deltaTime) {
    if (scene == nullptr || !isInitialized) {
        return;
    }

    // Audio system update logic would go here.
    audioImpl->Update(deltaTime);
}

void ME::AudioSystem::End() {
    if (audioImpl) {
        audioImpl->End();
        delete audioImpl;
        audioImpl = nullptr;
    }
    isInitialized = false;
}

void ME::AudioSystem::SetScene(Scene* scene) {
    this->scene = scene;   // Set the current scene.
    isInitialized = true;  // Mark the system as initialized.

    // Load audio resources related to the scene here.
}

void ME::AudioSystem::PlayMusic(uint8_t musicId, bool bLoop) {}

void ME::AudioSystem::StopMusic() {}

void ME::AudioSystem::PlayAudio(uint8_t soundId, bool bLoop) {}

void ME::AudioSystem::StopAudio(uint8_t soundId, bool bLoop) {}

void ME::AudioSystem::SetMasterVolume(float volume) {}

float ME::AudioSystem::GetMasterVolume() {
    return 0.0f;
}

void ME::AudioSystem::ToggleMuteAllSounds(bool bMute) {}
