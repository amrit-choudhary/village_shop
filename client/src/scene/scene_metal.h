#ifdef VG_MAC

/**
 * It takes a cross platform scene and converts it to a Metal scene.
 * This will create metal sepcific objects like buffers, textures, shaders, etc.
 */

#pragma once

#include <simd/simd.h>

#include <AppKit/AppKit.hpp>
#include <Metal/Metal.hpp>
#include <MetalKit/MetalKit.hpp>

#include "../rendering/metal/mesh_metal.h"
#include "../rendering/metal/quad_metal.h"
#include "../rendering/metal/texture_metal.h"
#include "scene.h"

namespace ME {

class SceneMetal {
   public:
    SceneMetal() = delete;
    SceneMetal(MTL::Device* device, MTL::CommandQueue* cmdQueue);
    ~SceneMetal();

    ME::Light* ambientLight;
    ME::Light* directionalLight;
    ME::Camera* camera;
    ME::Camera* spriteCamera;

    ME::MeshMetal** meshes;
    ME::QuadMetal** quads;
    ME::TextureMetal** textures;
    MTL::SamplerState** textureSamplerStates;
    ME::Transform** transforms;
    ME::MeshRenderer** meshRenderers;
    ME::Transform** spriteTransforms;
    ME::SpriteRenderer** spriteRenderers;

    uint16_t transformCount;
    uint16_t meshRendererCount;
    uint16_t spriteTransformCount;
    uint16_t spriteRendererCount;

   private:
    MTL::Device* device;
    MTL::CommandQueue* cmdQueue;
    ME::Scene* scene;

    void MakeMeshes();
    void MakeQuads();
    void MakeTextures();
    void MakeShaders();
    void MakeTextureSamplers();
};

}  // namespace ME

#endif
