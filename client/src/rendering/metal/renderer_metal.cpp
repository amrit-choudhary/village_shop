#include "renderer_metal.h"

#include <cmath>

#include "../shared/camera.h"
#include "../shared/color.h"
#include "../shared/image_loader_png.h"
#include "../shared/mesh.h"
#include "../shared/mesh_parser_obj.h"
#include "../shared/texture.h"
#include "../shared/transform.h"
#include "shader/shader_mac.h"
#include "src/logging.h"
#include "src/math/vec16.h"
#include "src/math/vec3.h"
#include "texture_metal.h"

void ME::RendererMetal::Init() {
    ME::Log("RendererMetal::Init");
}

void ME::RendererMetal::Update() {
    // Update.
}

void ME::RendererMetal::End() {
    commandQueue->release();
    device->release();
    view->release();
    delete scene;
}

void ME::RendererMetal::InitMTL(MTL::Device* inDevice, MTK::View* inView) {
    device = inDevice->retain();
    commandQueue = device->newCommandQueue();
    BuildScene();
    view = inView;
}

void ME::RendererMetal::BuildScene() {
    scene = new ME::SceneMetal(device, commandQueue);
}

void ME::RendererMetal::Draw(MTK::View* view) {
    NS::AutoreleasePool* pool = NS::AutoreleasePool::alloc()->init();

    MTL::CommandBuffer* cmd = commandQueue->commandBuffer();
    MTL::RenderPassDescriptor* rpd = view->currentRenderPassDescriptor();
    MTL::RenderCommandEncoder* enc = cmd->renderCommandEncoder(rpd);

    ME::LightDataAmbient lightDataAmbient = scene->ambientLight->GetLightDataAmbient();
    ME::LightDataDirectional lightDataDirectional = scene->directionalLight->GetLightDataDirectional();

    Vec16 viewMatVec = scene->camera->GetViewMatrix().GetData();
    Vec16 projectionMatVec = scene->camera->GetProjectionMatrix().GetData();

    enc->setRenderPipelineState(ME::RenderPipelineStateMetal::GetNewPSO(device));
    enc->setDepthStencilState(ME::DepthStencilStateMetal::GetNewDepthStencilState(device));

    enc->setVertexBuffer(scene->meshes[0]->vertexBuffer, 0, 0);
    enc->setVertexBytes(&viewMatVec, sizeof(ME::Vec16), 2);
    enc->setVertexBytes(&projectionMatVec, sizeof(ME::Vec16), 3);

    enc->setFragmentBytes(&lightDataAmbient, sizeof(ME::LightDataAmbient), 1);
    enc->setFragmentBytes(&lightDataDirectional, sizeof(ME::LightDataDirectional), 2);

    enc->setFragmentSamplerState(scene->textureSamplerStates[0], 0);

    enc->setCullMode(MTL::CullModeBack);
    enc->setFrontFacingWinding(MTL::Winding::WindingCounterClockwise);

    for (uint16_t i = 0; i < scene->meshRendererCount; ++i) {
        Vec16 modelMatVec = scene->transforms[i]->GetModelMatrix().GetData();
        enc->setVertexBytes(&modelMatVec, sizeof(ME::Vec16), 1);
        enc->setVertexBytes(&scene->meshRenderers[i]->color, sizeof(ME::Color), 4);
        enc->setFragmentTexture(scene->textures[scene->meshRenderers[i]->textureId]->GetTextureMetal(), 0);

        enc->drawIndexedPrimitives(MTL::PrimitiveType::PrimitiveTypeTriangle, scene->meshes[0]->indexCount,
                                   MTL::IndexType::IndexTypeUInt32, scene->meshes[0]->indexBuffer, 0, 1);
    }

    enc->endEncoding();
    cmd->presentDrawable(view->currentDrawable());
    cmd->commit();

    pool->release();
}
