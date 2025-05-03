#ifdef VG_MAC

/**
 * Metal's specific render pipeline state class.
 */

#pragma once

#include <simd/simd.h>

#include <AppKit/AppKit.hpp>
#include <Metal/Metal.hpp>
#include <MetalKit/MetalKit.hpp>

namespace ME {

/**
 * RenderPipelineStateMetal class for handling render pipeline state in Metal.
 * This class is used to create and manage the render pipeline state for Metal rendering.
 */
class RenderPipelineStateMetal {
   public:
    RenderPipelineStateMetal(MTL::Device* device);
    ~RenderPipelineStateMetal();

    MTL::RenderPipelineState* GetPSODefault() {
        return psoDefault;
    }

   private:
    void CreateDefaultStates(MTL::Device* device);

    MTL::RenderPipelineState* psoDefault;
};

}  // namespace ME

#endif
