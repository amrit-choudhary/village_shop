#pragma once
#ifdef VG_WIN

/**
 * It takes a cross platform scene and converts it to a DirectX 12 scene.
 * This will create DirectX specific objects like buffers, textures, shaders, etc.
 */

#include <d3d12.h>

#include "../rendering/directx/d3dx12.h"
#include "../rendering/directx/desc_heap_manager_dx.h"
#include "../rendering/directx/mesh_dx.h"
#include "../rendering/directx/quad_dx.h"
#include "../rendering/directx/texture_dx.h"
#include "../rendering/directx/upload_buffer_dx.h"
#include "scene.h"

namespace ME {

class SceneDX {
   public:
    SceneDX() = delete;
    SceneDX(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList, ME::DescHeapManagerDX* descHeapManager,
            ME::Scene* gameScene);
    ~SceneDX();

    // Init game
    void Init();

    // Cleanup upload buffers and other temp resources right after initialization.
    void PostInitCleanup();

    // Update scene. Mostly used to update the transforms and instance count. Actual scene change is done in the
    // ME::Scene class.
    void Update();

    void End();

    ME::Light* ambientLight;
    ME::Light* directionalLight;
    ME::Camera* camera;
    ME::Camera* spriteCamera;

    ME::MeshDX** meshes;
    ME::QuadDX** quads;
    ME::TextureDX** textures;
    ME::TextureDX** spriteTextures;
    ME::TextureAtlasProperties* textureAtlasProperties;
    CD3DX12_STATIC_SAMPLER_DESC** textureSamplerStates;
    ME::Transform** transforms;
    ME::MeshRenderer** meshRenderers;
    ME::Transform** spriteTransforms;
    ME::SpriteRenderer** spriteRenderers;
    ME::Transform** instancedSpriteTransforms;
    ME::SpriteRenderer** instancedSpriteRenderers;
    ME::Transform** textTransforms;
    ME::TextRenderer** textRenderers;

    uint16_t transformCount;
    uint16_t meshRendererCount;
    uint16_t spriteTransformCount;
    uint16_t spriteRendererCount;
    uint32_t instancedSpriteTransformCount;
    uint32_t instancedSpriteRendererCount;
    uint32_t textTransformsCount;
    uint32_t textRendererCount;
    uint32_t textInstanceDataCount;
    uint32_t perPassCBCount;

    ME::SpriteRendererInstanceData** spriteInstanceData;
    ME::UploadBufferDX* spriteInstanceBuffer = nullptr;

    ME::TextRendererInstanceData** textInstanceData;
    ME::UploadBufferDX* textInstanceBuffer = nullptr;
    D3D12_GPU_DESCRIPTOR_HANDLE textInstanceBufferHandle;

    ME::UploadBufferDX** perPassCBs = nullptr;

   private:
    ID3D12Device* device = nullptr;
    ID3D12GraphicsCommandList* cmdList = nullptr;
    ME::DescHeapManagerDX* descHeapManager = nullptr;
    ME::Scene* scene = nullptr;

    void MakeMeshes();
    void MakeQuads();
    void MakeTextures();
    void MakeSpriteTextures();
    void MakeShaders();
    void MakeTextureSamplers();
    void MakeConstantBuffers();

    void MakeSpriteInstanceBuffer();
    void MakeTextInstanceBuffer();
};

}  // namespace ME

#endif  // VG_WIN
