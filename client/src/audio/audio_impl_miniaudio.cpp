#include "audio_impl_miniaudio.h"

namespace ME {

void AudioImplMiniAudio::Init(AudioSystem* audioSystem) {}

void AudioImplMiniAudio::Update(double deltaTime) {}

void AudioImplMiniAudio::End() {}

void AudioImplMiniAudio::LoadSound(uint8_t soundId, const char* filePath) {}

void AudioImplMiniAudio::UnloadSound(uint8_t soundId) {}

void AudioImplMiniAudio::PlayMusic(uint8_t musicId, bool bLoop) {}

void AudioImplMiniAudio::StopMusic() {}

void AudioImplMiniAudio::PlaySound(uint8_t soundId, bool bLoop) {}

void AudioImplMiniAudio::StopSound(uint8_t soundId, bool bLoop) {}

void AudioImplMiniAudio::SetMasterVolume(float volume) {
    masterVolume = volume;
}

float AudioImplMiniAudio::GetMasterVolume() {
    return masterVolume;
}

void AudioImplMiniAudio::ToggleMuteAllSounds(bool bMute) {
    isMuted = bMute;
}

}  // namespace ME
