/**
 * Rendering for Metal platform.
 */

#pragma once

#include <simd/simd.h>

#include <AppKit/AppKit.hpp>
#include <Metal/Metal.hpp>
#include <MetalKit/MetalKit.hpp>

#include "../shared/renderer.h"

namespace ME {

/**
 * Renderer for Metal.
 * This class is used to render the game using Metal.
 */
class RendererMetal : public PlatformRenderer {
   public:
    // Init game
    void Init() override;

    // Will return true if game should tick based on FFR.
    void Update() override;

    // Will stop the timers to let it calculate average FPS.
    void End() override;

    void InitMTL(MTL::Device* device, MTK::View* view);

   private:
   protected:
   public:
    void BuildShaders();
    void BuildBuffers();
    void Draw(MTK::View* view);

   private:
    MTL::Device* device;
    MTK::View* view;
    MTL::CommandQueue* commandQueue;
    MTL::RenderPipelineState* PSO;
    MTL::Buffer* vertexBuffer;
    MTL::Buffer* indexBuffer;
};

}  // namespace ME
