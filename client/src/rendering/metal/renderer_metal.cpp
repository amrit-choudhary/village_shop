#include "renderer_metal.h"

#include "shader/shader_mac.h"
#include "src/logging.h"

void ME::RendererMetal::Init() {
    ME::Log("RendererMetal::Init");
}

void ME::RendererMetal::Update() {
    // Update.
}

void ME::RendererMetal::End() {
    vertexPositionsBuffer->release();
    vertexColorsBuffer->release();
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
    ME::Shader shader(device, "shaders/metal/first.shader");

    MTL::RenderPipelineDescriptor* desc = MTL::RenderPipelineDescriptor::alloc()->init();
    desc->setVertexFunction(shader.GetVertexFunction());
    desc->setFragmentFunction(shader.GetFragmentFunction());
    desc->colorAttachments()->object(0)->setPixelFormat(MTL::PixelFormat::PixelFormatBGRA8Unorm_sRGB);

    NS::Error* error = nullptr;
    PSO = device->newRenderPipelineState(desc, &error);
    if (!PSO) {
        __builtin_printf("%s", error->localizedDescription()->utf8String());
        assert(false);
    }

    desc->release();
}

void ME::RendererMetal::BuildBuffers() {
    const size_t NumVertices = 3;

    simd::float3 positions[NumVertices] = {{-0.8f, 0.8f, 0.0f}, {0.0f, -0.8f, 0.0f}, {+0.8f, 0.8f, 0.0f}};

    static float deltaR = 0;
    static float deltaG = 0;
    static float deltaB = 0;

    deltaR += 0.01;
    deltaG += 0.02;
    deltaB += 0.03;

    if (deltaR > 1.0f) deltaR = 0;
    if (deltaG > 1.0f) deltaG = 0;
    if (deltaB > 1.0f) deltaB = 0;

    simd::float3 colors[NumVertices] = {{deltaR, 0.0f, 0.0f}, {0.0, deltaG, 0.0f}, {0.0f, 0.0f, deltaB}};

    const size_t positionsDataSize = NumVertices * sizeof(simd::float3);
    const size_t colorDataSize = NumVertices * sizeof(simd::float3);

    MTL::Buffer* tempVertexPositionsBuffer = device->newBuffer(positionsDataSize, MTL::ResourceStorageModeManaged);
    MTL::Buffer* tempVertexColorsBuffer = device->newBuffer(colorDataSize, MTL::ResourceStorageModeManaged);

    vertexPositionsBuffer = tempVertexPositionsBuffer;
    vertexColorsBuffer = tempVertexColorsBuffer;

    memcpy(vertexPositionsBuffer->contents(), positions, positionsDataSize);
    memcpy(vertexColorsBuffer->contents(), colors, colorDataSize);

    vertexPositionsBuffer->didModifyRange(NS::Range::Make(0, vertexPositionsBuffer->length()));
    vertexColorsBuffer->didModifyRange(NS::Range::Make(0, vertexColorsBuffer->length()));
}

void ME::RendererMetal::Draw(MTK::View* view) {
    NS::AutoreleasePool* pool = NS::AutoreleasePool::alloc()->init();

    BuildBuffers();

    MTL::CommandBuffer* cmd = commandQueue->commandBuffer();
    MTL::RenderPassDescriptor* rpd = view->currentRenderPassDescriptor();
    MTL::RenderCommandEncoder* enc = cmd->renderCommandEncoder(rpd);

    enc->setRenderPipelineState(PSO);
    enc->setVertexBuffer(vertexPositionsBuffer, 0, 0);
    enc->setVertexBuffer(vertexColorsBuffer, 0, 1);
    enc->drawPrimitives(MTL::PrimitiveType::PrimitiveTypeTriangle, NS::UInteger(0), NS::UInteger(3));

    enc->endEncoding();
    cmd->presentDrawable(view->currentDrawable());
    cmd->commit();

    pool->release();
}
