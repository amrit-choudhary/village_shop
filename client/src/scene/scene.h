#pragma once

/**
 * Scene class implementation
 * This file contains the implementation of the Scene class, which is responsible for managing
 * the 3D scene, including loading models, managing lights, and rendering the scene.
 * The Scene class provides methods to load models, set up lights, and render the scene using
 * the rendering engine. It also handles the camera and view transformations.
 *
 * UI related functionalities are handled in a separate SceneUI class.
 */

#include "../rendering/shared/camera.h"
#include "../rendering/shared/light.h"
#include "../rendering/shared/material.h"
#include "../rendering/shared/mesh_renderer.h"
#include "../rendering/shared/sprite_renderer.h"
#include "../rendering/shared/text_renderer.h"
#include "../rendering/shared/texture.h"
#include "../rendering/shared/texture_sampler.h"
#include "src/physics/collider_aabb.h"

namespace ME {

class Scene {
   public:
    Scene();
    virtual ~Scene();

    /**
     * Updates the rendering related data in the scene, if any render element is marked as dirty.
     * This is called from Renderer.Update(), right before Renderer.Draw().
     * Like if sprite renderer's atlas index changed due to animation.
     * This is called every frame before rendering.
     * Everything that is not DX or MTL dependent is updated here, after which DX and MTL scene is updated from
     * Renderer.Update().
     */
    void Update();

   public:
    ME::Light* ambientLight;
    ME::Light* directionalLight;
    ME::Camera* camera;
    ME::Camera* spriteCamera;

    /**
     * Array of scene resources like meshes, textures, shaders, etc.
     * Items index in the array are file names and are used to create the mesh renderer.
     */
    const char** meshPaths = nullptr;
    const char** quadPaths = nullptr;
    const char** texturePaths = nullptr;
    const char** spriteTexturePaths = nullptr;
    const char** shaderPaths = nullptr;
    ME::TextureSampler* textureSamplers = nullptr;
    ME::TextureAtlasProperties* textureAtlasProperties = nullptr;
    ME::Transform** transforms = nullptr;
    ME::MeshRenderer** meshRenderers = nullptr;
    ME::Transform** spriteTransforms = nullptr;
    ME::SpriteRenderer** spriteRenderers = nullptr;

    // First set of instanced sprite renderers.
    ME::Transform** instancedSpriteTransforms0 = nullptr;
    ME::SpriteRenderer** instancedSpriteRenderers0 = nullptr;
    ME::SpriteRendererInstanceData* spriteInstanceData0 = nullptr;

    // Second set of instanced sprite renderers.
    ME::Transform** instancedSpriteTransforms1 = nullptr;
    ME::SpriteRenderer** instancedSpriteRenderers1 = nullptr;
    ME::SpriteRendererInstanceData* spriteInstanceData1 = nullptr;

    ME::ColliderAABB* staticColliders = nullptr;
    ME::ColliderAABB* dynamicColliders = nullptr;

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

    uint32_t instancedSpriteTransformCount0 = 0;
    uint32_t instancedSpriteRendererCount0 = 0;

    uint32_t instancedSpriteTransformCount1 = 0;
    uint32_t instancedSpriteRendererCount1 = 0;

    uint32_t staticColliderCount = 0;
    uint8_t dynamicColliderCount = 0;

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

   protected:
    // Helper functions to add elements to the scene.

    /**
     * Creates and adds a sprite transform with the given position and scale.
     * Will also increment the spriteTransformCount.
     */
    void AddSpriteTransform(ME::Vec3 position, ME::Vec3 scale);

    /**
     * Adds a sprite renderer to the spriteRenderers array.
     * Will also increment the spriteRendererCount.
     */
    void AddSpriteRenderer(ME::SpriteRenderer* spriteRenderer);

    /**
     * Creates and adds an instanced sprite transform with the given position and scale.
     * Will also increment the instancedSpriteTransformCount.
     * buffer specifies which instance buffer to add to (0 or 1).
     */
    void AddInstancedSpriteTransform(ME::Vec3 position, ME::Vec3 scale, uint8_t buffer = 0);

    /**
     * Adds an instanced sprite renderer to the instancedSpriteRenderers array.
     * Will also increment the instancedSpriteRendererCount.
     * buffer specifies which instance buffer to add to (0 or 1).
     */
    void AddInstancedSpriteRenderer(ME::SpriteRenderer* spriteRenderer, uint8_t buffer = 0);

   private:
    /**
     * Updates sprite renderers that are marked as dirty.
     * After update, the dirty flag is cleared.
     */
    void UpdateSpriteRenderers();

    /**
     * Updates instanced sprite renderers that are marked as dirty.
     * After update, the dirty flag is cleared.
     */
    void UpdateInstancedSpriteRenderers();
};

}  // namespace ME
