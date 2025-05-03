#ifdef VG_MAC

#include "render_pipeline_state_metal.h"

#include "../shared/vertex.h"
#include "shader/shader_mac.h"
#include "src/math/vec16.h"
#include "src/math/vec3.h"

ME::RenderPipelineStateMetal::RenderPipelineStateMetal(MTL::Device* device) {
    CreateDefaultStates(device);
}

ME::RenderPipelineStateMetal::~RenderPipelineStateMetal() {
    psoDefault->release();
}

void ME::RenderPipelineStateMetal::CreateDefaultStates(MTL::Device* device) {
    ME::Shader shader(device, "shaders/metal/second.metal");

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

    desc->setDepthAttachmentPixelFormat(MTL::PixelFormat::PixelFormatDepth32Float);

    NS::Error* error = nullptr;
    psoDefault = device->newRenderPipelineState(desc, &error);

    if (error) {
        __builtin_printf("Error creating render pipeline state: %s", error->localizedDescription()->utf8String());
        error->release();
    }

    if (psoDefault == nullptr) {
        __builtin_printf("Failed to create render pipeline state.");
    }

    vertexDesc->release();
    desc->release();
}

#endif
