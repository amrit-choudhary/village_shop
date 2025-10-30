/**
 * List of game constants.
 */

#pragma once

#include <cstdint>

namespace ME {
namespace Constants {

// Scene constants
constexpr uint8_t MaxMeshCount = 128;
constexpr uint8_t MaxQuadCount = 8;
constexpr uint8_t MaxTextureCount = 128;
constexpr uint8_t MaxSpriteTextureCount = 128;
constexpr uint8_t MaxTextureAtlasPropertiesCount = 128;
constexpr uint8_t MaxShaderCount = 128;
constexpr uint8_t MaxMaterialCount = 128;
constexpr uint8_t MaxSamplerCount = 16;
constexpr uint16_t MaxTransformCount = 1'000;
constexpr uint16_t MaxMeshRendererCount = 1'000;
constexpr uint16_t MaxSpriteTransformCount = 1'000;
constexpr uint16_t MaxSpriteRendererCount = 1'000;
constexpr uint32_t MaxInstancedSpriteTransformCount = 100'000;
constexpr uint32_t MaxInstancedSpriteRendererCount = 100'000;
constexpr uint16_t MaxTextTransformsCount = 1'000;
constexpr uint16_t MaxTextRendererCount = 1'000;
constexpr uint32_t MaxTextInstanceDataCount = 100'000;
constexpr uint32_t MaxConstantBuffersCount = 256;

// Physics constants
constexpr uint32_t MaxStaticColliderCount = 100'000;
constexpr uint32_t MaxDynamicColliderCount = 32;

}  // namespace Constants
}  // namespace ME
