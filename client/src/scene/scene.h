/**
 * Scene class implementation
 * This file contains the implementation of the Scene class, which is responsible for managing
 * the 3D scene, including loading models, managing lights, and rendering the scene.
 * The Scene class provides methods to load models, set up lights, and render the scene using
 * the rendering engine. It also handles the camera and view transformations.
 */

#pragma once

#include "../rendering/shared/camera.h"
#include "../rendering/shared/light.h"
#include "../rendering/shared/material.h"
#include "../rendering/shared/mesh_renderer.h"
#include "../rendering/shared/sprite_renderer.h"
#include "../rendering/shared/text_renderer.h"
#include "../rendering/shared/texture.h"
#include "../rendering/shared/texture_sampler.h"

namespace ME {

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

class Scene {
   public:
    Scene();
    virtual ~Scene();

   public:
    ME::Light* ambientLight;
    ME::Light* directionalLight;
    ME::Camera* camera;
    ME::Camera* spriteCamera;

    /**
     * Array of scene resources like meshes, textures, shaders, etc.
     * Items index in the array are file names and are used to create the mesh renderer.
     */
    const char** meshPaths;
    const char** quadPaths;
    const char** texturePaths;
    const char** spriteTexturePaths;
    const char** shaderPaths;
    ME::TextureSampler* textureSamplers;
    ME::Transform** transforms;
    ME::MeshRenderer** meshRenderers;
    ME::Transform** spriteTransforms;
    ME::SpriteRenderer** spriteRenderers;
    ME::Transform** instancedSpriteTransforms;
    ME::SpriteRenderer** instancedSpriteRenderers;
    ME::SpriteRendererInstanceData** spriteInstanceData;
    ME::TextureAtlasProperties* textureAtlasProperties;
    ME::Transform** textTransforms;
    ME::TextRenderer** textRenderers;
    ME::TextRendererInstanceData** textInstanceData;

    uint8_t meshCount = 0;
    uint8_t quadCount = 0;
    uint8_t textureCount = 0;
    uint8_t spriteTextureCount = 0;
    uint8_t textureAtlasPropertiesCount = 0;
    uint8_t shaderCount = 0;
    uint8_t textureSamplerCount = 0;
    uint16_t transformCount = 0;
    uint16_t meshRendererCount = 0;
    uint16_t spriteTransformCount = 0;
    uint16_t spriteRendererCount = 0;
    uint32_t instancedSpriteTransformCount = 0;
    uint32_t instancedSpriteRendererCount = 0;
    uint16_t textTransformsCount = 0;
    uint16_t textRendererCount = 0;
    uint32_t textInstanceDataCount = 0;

    virtual void Init();
    virtual void CreateResources();
    virtual void BuildLights();
    virtual void BuildCamera();
    virtual void BuildTransforms();
    virtual void BuildMeshRenderers();
    virtual void BuildSpriteTransforms();
    virtual void BuildSpriteRenderers();
    virtual void BuildInstancedSpriteTransforms();
    virtual void BuildInstancedSpriteRenderers();
    virtual void BuildTextRenderers();
};

}  // namespace ME
