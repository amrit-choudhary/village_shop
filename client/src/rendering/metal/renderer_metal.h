/**
 * Rendering for Metal platform.
 */

#pragma once

#include <simd/simd.h>

#include <AppKit/AppKit.hpp>
#include <Metal/Metal.hpp>
#include <MetalKit/MetalKit.hpp>

#include "../shared/renderer.h"
#include "depth_stencil_state_metal.h"
#include "mesh_metal.h"
#include "render_pipeline_state_metal.h"
#include "texture_metal.h"

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
    void BuildDepthStencilState();
    void BuildBuffers();
    void BuildTextures();
    void Draw(MTK::View* view);

   private:
    MTL::Device* device;
    MTK::View* view;
    MTL::CommandQueue* commandQueue;
    MTL::Buffer* instanceBuffer;
    ME::TextureMetal* texture1;
    ME::TextureMetal* texture2;
    ME::TextureMetal* texture3;
    ME::TextureMetal* texture4;
    MTL::SamplerState* samplerState;
    ME::RenderPipelineStateMetal* renderPipelineState;
    ME::DepthStencilStateMetal* depthStencilState;
    ME::MeshMetal* mesh;

    size_t instanceCount = 0;
};

}  // namespace ME
