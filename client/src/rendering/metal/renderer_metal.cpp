#include "renderer_metal.h"

#include <cmath>

#include "../shared/mesh.h"
#include "../shared/mesh_parser_obj.h"
#include "shader/shader_mac.h"
#include "src/logging.h"
#include "src/math/vec16.h"
#include "src/math/vec3.h"

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
    modelBuffer->release();
    projectionBuffer->release();
    PSO->release();
    commandQueue->release();
    device->release();
    view->release();
}

void ME::RendererMetal::InitMTL(MTL::Device* inDevice, MTK::View* inView) {
    device = inDevice->retain();
    commandQueue = device->newCommandQueue();
    BuildShaders();
    BuildBuffers();
    view = inView;
}

void ME::RendererMetal::BuildShaders() {
    ME::Shader shader(device, "shaders/metal/first.metal");

    MTL::RenderPipelineDescriptor* desc = MTL::RenderPipelineDescriptor::alloc()->init();
    desc->setVertexFunction(shader.GetVertexFunction());
    desc->setFragmentFunction(shader.GetFragmentFunction());
    desc->colorAttachments()->object(0)->setPixelFormat(MTL::PixelFormat::PixelFormatBGRA8Unorm_sRGB);

    MTL::VertexDescriptor* vertexDesc = MTL::VertexDescriptor::alloc()->init();

    // Position
    vertexDesc->attributes()->object(0)->setFormat(MTL::VertexFormat::VertexFormatFloat3);
    vertexDesc->attributes()->object(0)->setOffset(0);
    vertexDesc->attributes()->object(0)->setBufferIndex(0);
    // Normal
    vertexDesc->attributes()->object(1)->setFormat(MTL::VertexFormat::VertexFormatFloat3);
    vertexDesc->attributes()->object(1)->setOffset(12);
    vertexDesc->attributes()->object(1)->setBufferIndex(0);
    // UV
    vertexDesc->attributes()->object(2)->setFormat(MTL::VertexFormat::VertexFormatFloat2);
    vertexDesc->attributes()->object(2)->setOffset(24);
    vertexDesc->attributes()->object(2)->setBufferIndex(0);

    vertexDesc->layouts()->object(0)->setStride(sizeof(ME::Vertex));
    vertexDesc->layouts()->object(0)->setStepFunction(MTL::VertexStepFunction::VertexStepFunctionPerVertex);

    desc->setVertexDescriptor(vertexDesc);

    NS::Error* error = nullptr;
    PSO = device->newRenderPipelineState(desc, &error);
    if (!PSO) {
        __builtin_printf("%s", error->localizedDescription()->utf8String());
        assert(false);
    }

    vertexDesc->release();
    desc->release();
}

void ME::RendererMetal::BuildBuffers() {
    Mesh mesh = ME::CreateMeshFromOBJ("meshes/cube.obj");
    // mesh.CalculateNormal();

    const size_t vertexCount = mesh.vertexCount;
    const size_t indexCount = mesh.indexCount;

    const size_t vertexDataSize = vertexCount * sizeof(ME::Vertex);
    const size_t indexDataSize = indexCount * sizeof(uint32_t);

    vertexBuffer = device->newBuffer(vertexDataSize, MTL::ResourceStorageModeManaged);
    indexBuffer = device->newBuffer(indexDataSize, MTL::ResourceStorageModeManaged);
    modelBuffer = device->newBuffer(sizeof(Vec16), MTL::ResourceStorageModeManaged);
    viewBuffer = device->newBuffer(sizeof(Vec16), MTL::ResourceStorageModeManaged);
    projectionBuffer = device->newBuffer(sizeof(Vec16), MTL::ResourceStorageModeManaged);

    memcpy(vertexBuffer->contents(), mesh.vertices.data(), vertexDataSize);
    memcpy(indexBuffer->contents(), mesh.indices.data(), indexDataSize);

    vertexBuffer->didModifyRange(NS::Range::Make(0, vertexBuffer->length()));
    indexBuffer->didModifyRange(NS::Range::Make(0, indexBuffer->length()));
}

void ME::RendererMetal::Draw(MTK::View* view) {
    NS::AutoreleasePool* pool = NS::AutoreleasePool::alloc()->init();

    static float rotation = 0.0f;
    rotation += 0.01f;
    if (rotation > 360.0f) {
        rotation = 0.0f;
    }

    Mat4 translationMat = Mat4::Translation(Vec4(0.0f, 0.0f, 20.0f, 1.0f));
    Mat4 rotationMat = Mat4::Rotation(Vec4(0.0f, rotation, 0.0f, 1.0f));
    Mat4 scaleMat = Mat4::Scale(Vec4(10.0f, 10.0f, 10.0f, 1.0f));
    Mat4 modelMat = translationMat * rotationMat * scaleMat;

    Vec16 modelData = modelMat.GetData();
    memcpy(modelBuffer->contents(), &modelData, sizeof(Vec16));
    modelBuffer->didModifyRange(NS::Range::Make(0, modelBuffer->length()));

    Mat4 viewMat = Mat4::Identity;
    Vec16 viewData = viewMat.GetData();
    memcpy(viewBuffer->contents(), &viewData, sizeof(Vec16));
    viewBuffer->didModifyRange(NS::Range::Make(0, viewBuffer->length()));

    Mat4 projectionMat = Mat4::Perspective(90.0f * (M_PI / 180.0f), 1.0f, 10.0f, 100.0f);
    // Mat4 projectionMat = Mat4::Identity;
    Vec16 projectionData = projectionMat.GetData();
    memcpy(projectionBuffer->contents(), &projectionData, sizeof(Vec16));
    projectionBuffer->didModifyRange(NS::Range::Make(0, projectionBuffer->length()));

    MTL::CommandBuffer* cmd = commandQueue->commandBuffer();
    MTL::RenderPassDescriptor* rpd = view->currentRenderPassDescriptor();
    MTL::RenderCommandEncoder* enc = cmd->renderCommandEncoder(rpd);

    enc->setRenderPipelineState(PSO);
    enc->setVertexBuffer(vertexBuffer, 0, 0);
    enc->setVertexBuffer(modelBuffer, 0, 1);
    enc->setVertexBuffer(viewBuffer, 0, 2);
    enc->setVertexBuffer(projectionBuffer, 0, 3);

    enc->setCullMode(MTL::CullModeBack);
    enc->setFrontFacingWinding(MTL::Winding::WindingCounterClockwise);

    enc->drawIndexedPrimitives(MTL::PrimitiveType::PrimitiveTypeTriangle, indexBuffer->length() / sizeof(uint32_t),
                               MTL::IndexType::IndexTypeUInt32, indexBuffer, 0, 1);

    enc->endEncoding();
    cmd->presentDrawable(view->currentDrawable());
    cmd->commit();

    pool->release();
}
