#ifdef VG_MAC
/**
 * Metal's specific depth stencil state class.
 */

#pragma once

#include <simd/simd.h>

#include <AppKit/AppKit.hpp>
#include <Metal/Metal.hpp>
#include <MetalKit/MetalKit.hpp>

namespace ME {

/**
 * DepthStencilStateMetal class for handling depth stencil state in Metal.
 * This class is used to create and manage the depth stencil state for Metal rendering.
 */
class DepthStencilStateMetal {
   public:
    DepthStencilStateMetal(MTL::Device* device);
    ~DepthStencilStateMetal();

    MTL::DepthStencilState* GetDSSDefault() {
        return dssDefault;
    }

   private:
    void CreateDefaultStates(MTL::Device* device);

    MTL::DepthStencilState* dssDefault;
};

}  // namespace ME

#endif
