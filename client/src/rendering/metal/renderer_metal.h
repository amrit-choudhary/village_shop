/**
 * Rendering for Metal platform.
 */

#pragma once

#include <simd/simd.h>

#include <AppKit/AppKit.hpp>
#include <Metal/Metal.hpp>
#include <MetalKit/MetalKit.hpp>

#include "../../src/scene/scene_metal.h"
#include "../shared/light.h"
#include "../shared/renderer.h"
#include "depth_stencil_state_metal.h"
#include "mesh_metal.h"
#include "quad_metal.h"
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

    // Sets scene for the renderer.
    void SetScene(ME::Scene* gameScene) override;

    // Will return true if game should tick based on FFR.
    void Update() override;

    // Will stop the timers to let it calculate average FPS.
    void End() override;

    void InitMTL(MTL::Device* device, MTK::View* view);

   private:
   protected:
   public:
    void Draw(MTK::View* view);

   private:
    MTL::Device* device;
    MTK::View* view;
    MTL::CommandQueue* commandQueue;
    ME::SceneMetal* scene;
};

}  // namespace ME
