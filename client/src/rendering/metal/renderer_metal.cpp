#include "renderer_metal.h"

#include <cmath>

#include "../shared/camera.h"
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

using namespace ME::Math;

void ME::RendererMetal::Init() {
    ME::Log("RendererMetal::Init");
}

void ME::RendererMetal::Update() {
    // Update.
}

void ME::RendererMetal::End() {
    vertexBuffer->release();
    indexBuffer->release();
    instanceBuffer->release();
    samplerState->release();
    commandQueue->release();
    device->release();
    view->release();
    delete texture1;
    delete texture2;
    delete texture3;
    delete texture4;
    delete renderPipelineState;
    delete depthStencilState;
}

void ME::RendererMetal::InitMTL(MTL::Device* inDevice, MTK::View* inView) {
    device = inDevice->retain();
    commandQueue = device->newCommandQueue();
    BuildShaders();
    BuildDepthStencilState();
    BuildBuffers();
    BuildTextures();
    view = inView;
}

void ME::RendererMetal::BuildShaders() {
    renderPipelineState = new ME::RenderPipelineStateMetal(device);
}

void ME::RendererMetal::BuildDepthStencilState() {
    depthStencilState = new ME::DepthStencilStateMetal(device);
}

void ME::RendererMetal::BuildBuffers() {
    Mesh mesh = ME::CreateMeshFromOBJ("meshes/cube_unshared.obj");
    // mesh.CalculateNormal();

    const size_t vertexCount = mesh.vertexCount;
    const size_t indexCount = mesh.indexCount;

    const size_t vertexDataSize = vertexCount * sizeof(ME::Vertex);
    const size_t indexDataSize = indexCount * sizeof(uint32_t);

    vertexBuffer = device->newBuffer(vertexDataSize, MTL::ResourceStorageModeManaged);
    indexBuffer = device->newBuffer(indexDataSize, MTL::ResourceStorageModeManaged);

    memcpy(vertexBuffer->contents(), mesh.vertices.data(), vertexDataSize);
    vertexBuffer->didModifyRange(NS::Range::Make(0, vertexBuffer->length()));
    memcpy(indexBuffer->contents(), mesh.indices.data(), indexDataSize);
    indexBuffer->didModifyRange(NS::Range::Make(0, indexBuffer->length()));

    ME::Texture heightmap{"textures/world/heightmap.png"};
    const uint32_t w = heightmap.GetWidth();
    const uint32_t h = heightmap.GetHeight();
    const uint8_t* data = heightmap.GetData();
    instanceCount = w * h;
    Vec4 instanceData[instanceCount];

    for (size_t i = 0; i < instanceCount; ++i) {
        instanceData[i] =
            Vec4{static_cast<float>(i % w), 12.0f * data[i * 4] / 255.0f, static_cast<float>(i / w), 1.0f};
    }

    instanceBuffer = device->newBuffer(sizeof(Vec4) * instanceCount, MTL::ResourceStorageModeManaged);
    memcpy(instanceBuffer->contents(), instanceData, sizeof(Vec4) * instanceCount);
    instanceBuffer->didModifyRange(NS::Range::Make(0, instanceBuffer->length()));
}

void ME::RendererMetal::BuildTextures() {
    texture1 = new ME::TextureMetal{"textures/world/cobblestone.png", device, true, commandQueue};
    texture2 = new ME::TextureMetal{"textures/world/dirt.png", device, true, commandQueue};
    texture3 = new ME::TextureMetal{"textures/world/ice.png", device, true, commandQueue};
    texture4 = new ME::TextureMetal{"textures/world/green_concrete_powder.png", device, true, commandQueue};

    samplerState = ME::TextureMetal::GetSamplerState(device);
}

void ME::RendererMetal::Draw(MTK::View* view) {
    NS::AutoreleasePool* pool = NS::AutoreleasePool::alloc()->init();

    static float rotation = 0.0f;
    static float translation = 0.0f;
    rotation += 0.01f;
    translation += 0.1f;
    if (rotation > 360.0f) {
        rotation = 0.0f;
    }

    ME::Transform transform;
    transform.SetPosition(0.0f, 0.0f, 0.0f);
    transform.SetRotation(0.0f, 0.0f, 0.0f, 1.0f);
    transform.SetScale(1.0f, 3.0f, 1.0f);
    Vec16 modelMatVec = transform.GetModelMatrix().GetData();

    ME::Camera camera;
    camera.position = ME::Math::Vec3{-10.0f + translation, 50.0f, -10.0f + translation};
    camera.viewPosition = ME::Math::Vec3{200.0f, 0.0f, 200.0f};
    Vec16 viewMatVec = camera.GetViewMatrix().GetData();

    Vec16 projectionMatVec = camera.GetProjectionMatrix().GetData();

    MTL::CommandBuffer* cmd = commandQueue->commandBuffer();
    MTL::RenderPassDescriptor* rpd = view->currentRenderPassDescriptor();
    MTL::RenderCommandEncoder* enc = cmd->renderCommandEncoder(rpd);

    enc->setRenderPipelineState(renderPipelineState->GetPSODefault());
    enc->setDepthStencilState(depthStencilState->GetDSSDefault());

    enc->setVertexBuffer(vertexBuffer, 0, 0);
    enc->setVertexBytes(&modelMatVec, sizeof(ME::Math::Vec16), 1);
    enc->setVertexBytes(&viewMatVec, sizeof(ME::Math::Vec16), 2);
    enc->setVertexBytes(&projectionMatVec, sizeof(ME::Math::Vec16), 3);
    enc->setVertexBuffer(instanceBuffer, 0, 4);

    enc->setFragmentTexture(texture1->GetTextureMetal(), 0);
    enc->setFragmentTexture(texture2->GetTextureMetal(), 1);
    enc->setFragmentTexture(texture3->GetTextureMetal(), 2);
    enc->setFragmentTexture(texture4->GetTextureMetal(), 3);

    enc->setFragmentSamplerState(samplerState, 0);

    enc->setCullMode(MTL::CullModeBack);
    enc->setFrontFacingWinding(MTL::Winding::WindingCounterClockwise);

    enc->drawIndexedPrimitives(MTL::PrimitiveType::PrimitiveTypeTriangle, indexBuffer->length() / sizeof(uint32_t),
                               MTL::IndexType::IndexTypeUInt32, indexBuffer, 0, instanceCount);

    enc->endEncoding();
    cmd->presentDrawable(view->currentDrawable());
    cmd->commit();

    pool->release();
}
