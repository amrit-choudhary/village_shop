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
#include "../rendering/shared/texture_sampler.h"

namespace ME {

constexpr uint8_t MaxMeshCount = 128;
constexpr uint8_t MaxTextureCount = 128;
constexpr uint8_t MaxShaderCount = 128;
constexpr uint8_t MaxMaterialCount = 128;
constexpr uint8_t MaxSamplerCount = 16;

class Scene {
   public:
    Scene();
    ~Scene();

   public:
    ME::Light* ambientLight;
    ME::Light* directionalLight;
    ME::Camera* camera;
    /**
     * Array of scene resources like meshes, textures, shaders, etc.
     * Items index in the array are file names and are used to create the mesh renderer.
     */
    const char** meshPaths;
    const char** texturePaths;
    const char** shaderPaths;
    ME::TextureSampler* textureSamplers;

    uint8_t meshCount;
    uint8_t textureCount;
    uint8_t shaderCount;
    uint8_t textureSamplerCount;

    void CreateResources();
    void BuildLights();
    void BuildCamera();
    void BuildMeshRenderers();
};

}  // namespace ME
