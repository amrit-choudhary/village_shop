/**
 * List of game constants.
 */

#pragma once

#include <cstdint>

namespace ME {
namespace Constants {

// Scene constants
constexpr size_t MaxMeshCount = 128;
constexpr size_t MaxQuadCount = 8;
constexpr size_t MaxTextureCount = 128;
constexpr size_t MaxSpriteTextureCount = 128;
constexpr size_t MaxTextureAtlasPropertiesCount = 128;
constexpr size_t MaxShaderCount = 128;
constexpr size_t MaxMaterialCount = 128;
constexpr size_t MaxSamplerCount = 16;
constexpr size_t MaxTransformCount = 1'000;
constexpr size_t MaxMeshRendererCount = 1'000;

constexpr size_t MaxSpriteTransformCount = 1'000;
constexpr size_t MaxSpriteRendererCount = 1'000;

constexpr size_t MaxInstancedSpriteTransformCount = 10'000;
constexpr size_t MaxInstancedSpriteRendererCount = 10'000;
constexpr size_t MaxSpriteInstanceDataCount = 10'000;

constexpr size_t MaxUISpriteTransformCount = 10'000;
constexpr size_t MaxUISpriteRendererCount = 10'000;
constexpr size_t MaxUISpriteInstanceDataCount = 10'000;

constexpr size_t MaxTextTransformsCount = 1'000;
constexpr size_t MaxTextRendererCount = 1'000;
constexpr size_t MaxTextInstanceDataCount = 10'000;

constexpr size_t MaxConstantBuffersCount = 256;
constexpr size_t MaxDescriptorsOnDescriptorHeap = 256;

// Physics constants
constexpr size_t MaxStaticColliderCount = 100'000;
constexpr size_t MaxDynamicColliderCount = 100'000;
constexpr size_t MaxPhysicsLayerCount = 16;

// Animation constants
constexpr size_t AnimGameFrameRate = 60;
constexpr size_t MaxSpriteAnimClipsOnAnimator = 8;

// Audio constants
constexpr size_t MaxSimultaneousSounds = 32;
constexpr size_t MaxAudioSources = 64;
constexpr size_t MaxAudioListeners = 4;
constexpr size_t MaxLoadedSFXCount = 64;
constexpr size_t MaxLoadedMusicCount = 16;

}  // namespace Constants
}  // namespace ME
