#include "renderer_metal.h"

#include <cmath>

#include "../shared/mesh.h"
#include "../shared/mesh_parser_obj.h"
#include "shader/shader_mac.h"
#include "src/logging.h"
#include "src/math/vec3.h"

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
    ME::Shader shader(device, "shaders/metal/first.metal");

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
    static float rot = 0;
    rot += 0.03f;

    Mesh mesh = ME::CreateMeshFromOBJ("meshes/icosahedron.obj");
    const size_t count = mesh.indexCount;

    simd::float3 positions[count];
    simd::float3 colors[count];
    for (int i = 0; i < count; i++) {
        ME::Vertex vertex = mesh.vertices[mesh.indices[i]];
        ME::Math::Vec3 rotatedPosition =
            ME::Math::Vec3(vertex.position.x * cos(rot) - vertex.position.z * sin(rot), vertex.position.y,
                           vertex.position.x * sin(rot) + vertex.position.z * cos(rot));

        positions[i] = {rotatedPosition.x, rotatedPosition.y, rotatedPosition.z};

        ME::Math::Vec3 normNorm = vertex.normal.Normalised();

        colors[i] = {vertex.uv.x, vertex.uv.y, 0.0f};
        // colors[i] = {vertex.normal.x, vertex.normal.y, vertex.normal.z};
    }

    const size_t positionsDataSize = count * sizeof(simd::float3);
    const size_t colorDataSize = count * sizeof(simd::float3);

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

    enc->setCullMode(MTL::CullModeBack);
    enc->setFrontFacingWinding(MTL::Winding::WindingCounterClockwise);

    enc->drawPrimitives(MTL::PrimitiveType::PrimitiveTypeTriangle, NS::UInteger(0), NS::UInteger(100));

    enc->endEncoding();
    cmd->presentDrawable(view->currentDrawable());
    cmd->commit();

    pool->release();
}
