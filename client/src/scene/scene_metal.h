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

#include "scene.h"

namespace ME {

class SceneMetal {
   public:
    SceneMetal() = delete;
    SceneMetal(ME::Scene* scene, MTL::Device* device, MTL::CommandQueue* cmdQueue);
    ~SceneMetal();

    ME::Light* ambientLight;
    ME::Light* directionalLight;
    ME::Camera* camera;

   private:
    MTL::Device* device;
    MTL::CommandQueue* cmdQueue;
};

}  // namespace ME

#endif
