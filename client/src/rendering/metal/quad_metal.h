#pragma once
#ifdef VG_MAC

/**
 * Metal's specific quad class.
 * Has MTL buffers for vertex, index and instance data.
 * This class is used to load and manage quads using Metal.
 */

#include <simd/simd.h>

#include <AppKit/AppKit.hpp>
#include <Metal/Metal.hpp>
#include <MetalKit/MetalKit.hpp>

#include "../shared/quad.h"

namespace ME {

class QuadMetal : public Quad {
   public:
    /**
     * 1. Create Metal buffers for vertex, index and instance data.
     * 2. Upload the data to the GPU.
     */
    QuadMetal(const char* path, MTL::Device* device, MTL::CommandQueue* cmdQueue);
    ~QuadMetal() override;

    MTL::Buffer* vertexBuffer;
    MTL::Buffer* indexBuffer;
};

}  // namespace ME

#endif
