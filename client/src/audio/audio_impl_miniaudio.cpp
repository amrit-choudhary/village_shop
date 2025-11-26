#include "audio_impl_miniaudio.h"

#include "../../third_party/miniaudio-0.11.23/miniaudio.h"

namespace ME {

static ma_engine g_engine;
static ma_sound g_music;
static bool g_engineInitialized = false;

void AudioImplMiniaudio::Init(AudioSystem* audioSystem) {
    // very simple: init miniaudio engine and play one-shot laser sound
    if (ma_engine_init(NULL, &g_engine) != MA_SUCCESS) {
        return;
    }

    g_engineInitialized = true;

    ma_engine_play_sound(&g_engine, "resources/audio/sfx/laser.wav", NULL);

    // looping music: load into a ma_sound, set looping and start
    if (ma_sound_init_from_file(&g_engine, "resources/audio/music/ambience.wav", 0, NULL, NULL, &g_music) ==
        MA_SUCCESS) {
        ma_sound_set_looping(&g_music, MA_TRUE);  // MA_TRUE (miniaudio) sets loop
        ma_sound_start(&g_music);
    }
}

void AudioImplMiniaudio::Update(double deltaTime) {}

void AudioImplMiniaudio::End() {
    if (g_engineInitialized) {
        ma_sound_uninit(&g_music);
        ma_engine_uninit(&g_engine);
        g_engineInitialized = false;
    }
}

void AudioImplMiniaudio::LoadAudioFile(uint8_t soundId, const char* filePath) {}

void AudioImplMiniaudio::UnloadAudioFile(uint8_t soundId) {}

void AudioImplMiniaudio::PlayMusic(uint8_t musicId, bool bLoop) {}

void AudioImplMiniaudio::StopMusic() {}

void AudioImplMiniaudio::PlayAudio(uint8_t soundId, bool bLoop) {}

void AudioImplMiniaudio::StopAudio(uint8_t soundId, bool bLoop) {}

void AudioImplMiniaudio::SetMasterVolume(float volume) {
    masterVolume = volume;
}

float AudioImplMiniaudio::GetMasterVolume() {
    return masterVolume;
}

void AudioImplMiniaudio::ToggleMuteAllSounds(bool bMute) {
    isMuted = bMute;
}

}  // namespace ME
