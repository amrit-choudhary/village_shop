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
#include "../rendering/shared/texture_sampler.h"

namespace ME {

constexpr uint8_t MaxMeshCount = 128;
constexpr uint8_t MaxQuadCount = 8;
constexpr uint8_t MaxTextureCount = 128;
constexpr uint8_t MaxSpriteTextureCount = 128;
constexpr uint8_t MaxShaderCount = 128;
constexpr uint8_t MaxMaterialCount = 128;
constexpr uint8_t MaxSamplerCount = 16;
constexpr uint16_t MaxTransformCount = 1'000;
constexpr uint16_t MaxMeshRendererCount = 1'000;
constexpr uint16_t MaxSpriteTransformCount = 1'000;
constexpr uint16_t MaxSpriteRendererCount = 1'000;
constexpr uint32_t MaxInstancedSpriteTransformCount = 100'000;
constexpr uint32_t MaxInstancedSpriteRendererCount = 100'000;

class Scene {
   public:
    Scene();
    ~Scene();

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

    uint8_t meshCount;
    uint8_t quadCount;
    uint8_t textureCount;
    uint8_t spriteTextureCount;
    uint8_t shaderCount;
    uint8_t textureSamplerCount;
    uint16_t transformCount;
    uint16_t meshRendererCount;
    uint16_t spriteTransformCount;
    uint16_t spriteRendererCount;
    uint32_t instancedSpriteTransformCount;
    uint32_t instancedSpriteRendererCount;

    void CreateResources();
    void BuildLights();
    void BuildCamera();
    void BuildTransforms();
    void BuildMeshRenderers();
    void BuildSpriteTransforms();
    void BuildSpriteRenderers();
    void BuildInstancedSpriteTransforms();
    void BuildInstancedSpriteRenderers();
};

}  // namespace ME
