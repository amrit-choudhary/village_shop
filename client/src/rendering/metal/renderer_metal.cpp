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
    samplerState->release();
    commandQueue->release();
    device->release();
    view->release();
    delete texture1;
    delete renderPipelineState;
    delete depthStencilState;
    delete mesh;
    delete scene;
}

void ME::RendererMetal::InitMTL(MTL::Device* inDevice, MTK::View* inView) {
    device = inDevice->retain();
    commandQueue = device->newCommandQueue();
    BuildShaders();
    BuildDepthStencilState();
    BuildBuffers();
    BuildTextures();
    BuildScene();
    view = inView;
}

void ME::RendererMetal::BuildShaders() {
    renderPipelineState = new ME::RenderPipelineStateMetal(device);
}

void ME::RendererMetal::BuildDepthStencilState() {
    depthStencilState = new ME::DepthStencilStateMetal(device);
}

void ME::RendererMetal::BuildBuffers() {
    mesh = new ME::MeshMetal("meshes/cube_unshared.obj", device, commandQueue);
}

void ME::RendererMetal::BuildTextures() {
    texture1 = new ME::TextureMetal{"textures/world/cobblestone.png", device, true, commandQueue};

    samplerState = ME::TextureMetal::GetSamplerStateNearest(device);
}

void ME::RendererMetal::BuildScene() {
    scene = new ME::Scene();
}

void ME::RendererMetal::Draw(MTK::View* view) {
    NS::AutoreleasePool* pool = NS::AutoreleasePool::alloc()->init();

    ME::Color tint = ME::Color::White();

    static float rotation = 0.0f;
    static float translation = 0.0f;
    rotation += 0.01f;
    //     translation += 0.1f;
    if (rotation > 360.0f) {
        rotation = 0.0f;
    }

    ME::LightDataAmbient lightDataAmbient = scene->ambientLight->GetLightDataAmbient();
    ME::LightDataDirectional lightDataDirectional = scene->directionalLight->GetLightDataDirectional();

    ME::Transform transform;
    transform.SetPosition(0.0f, 0.0f, 0.0f);
    transform.SetRotation(rotation, rotation, 0.0f, 1.0f);
    transform.SetScale(3.0f, 3.0f, 3.0f);
    Vec16 modelMatVec = transform.GetModelMatrix().GetData();

    Vec16 viewMatVec = scene->camera->GetViewMatrix().GetData();
    Vec16 projectionMatVec = scene->camera->GetProjectionMatrix().GetData();

    MTL::CommandBuffer* cmd = commandQueue->commandBuffer();
    MTL::RenderPassDescriptor* rpd = view->currentRenderPassDescriptor();
    MTL::RenderCommandEncoder* enc = cmd->renderCommandEncoder(rpd);

    enc->setRenderPipelineState(renderPipelineState->GetPSODefault());
    enc->setDepthStencilState(depthStencilState->GetDSSDefault());

    enc->setVertexBuffer(mesh->vertexBuffer, 0, 0);
    enc->setVertexBytes(&modelMatVec, sizeof(ME::Vec16), 1);
    enc->setVertexBytes(&viewMatVec, sizeof(ME::Vec16), 2);
    enc->setVertexBytes(&projectionMatVec, sizeof(ME::Vec16), 3);
    enc->setVertexBytes(&tint, sizeof(ME::Color), 4);

    enc->setFragmentTexture(texture1->GetTextureMetal(), 0);
    enc->setFragmentBytes(&lightDataAmbient, sizeof(ME::LightDataAmbient), 1);
    enc->setFragmentBytes(&lightDataDirectional, sizeof(ME::LightDataDirectional), 2);

    enc->setFragmentSamplerState(samplerState, 0);

    enc->setCullMode(MTL::CullModeBack);
    enc->setFrontFacingWinding(MTL::Winding::WindingCounterClockwise);

    enc->drawIndexedPrimitives(MTL::PrimitiveType::PrimitiveTypeTriangle, mesh->indexCount,
                               MTL::IndexType::IndexTypeUInt32, mesh->indexBuffer, 0, 1);

    enc->endEncoding();
    cmd->presentDrawable(view->currentDrawable());
    cmd->commit();

    pool->release();
}
