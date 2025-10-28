#pragma once
#ifdef VG_WIN

/**
 * Renderer class for DirectX 12 on Windows.
 */

#include <d3d12.h>
#include <dxgi1_6.h>
#include <windows.h>
#include <wrl.h>

#include "../shared/renderer.h"

using Microsoft::WRL::ComPtr;

namespace ME {

// Forward declarations to avoid including shader/quad headers here.
class SceneDX;
class Shader;
class QuadDX;
class UploadBufferDX;
class MeshDX;
class Camera;
class Light;
class TextureDX;
class SpriteRendererInstanceData;

/**
 * Renderer class for DirectX 12 on Windows.
 */
class RendererDX : public PlatformRenderer {
   public:
    // Init game
    void Init() override;

    // Sets scene for the renderer.
    void SetScene(ME::Scene* gameScene) override;

    // Will return true if game should tick based on FFR.
    void Update() override;

    // Will stop the timers to let it calculate average FPS.
    void End() override;

    // Initialize DirectX 12.
    bool InitDX(HWND hwnd);

    void Draw();

   private:
    HWND hWnd;
    ME::SceneDX* scene = nullptr;
    ComPtr<IDXGIFactory4> factory;
    ComPtr<ID3D12Device> device;
    ComPtr<ID3D12CommandQueue> commandQueue;
    ComPtr<IDXGISwapChain1> swapChain;
    ComPtr<ID3D12Fence> fence;
    uint64_t currentFence = 0;

    ComPtr<ID3D12CommandAllocator> directCmdListAlloc;
    ComPtr<ID3D12GraphicsCommandList> commandList;

    static const uint8_t swapChainBufferCount = 2;
    uint8_t currentBackBuffer = 0;
    ComPtr<ID3D12Resource> swapChainBuffers[swapChainBufferCount];
    ComPtr<ID3D12Resource> depthStencilBuffer;

    D3D12_VIEWPORT screenViewport;
    D3D12_RECT scissorRect;

    ID3D12PipelineState* pso = nullptr;
    ID3D12RootSignature* rootSignature = nullptr;

    uint32_t rtvDescriptorSize = 0;
    uint32_t dsvDescriptorSize = 0;
    uint32_t cbvSrvUavDescriptorSize = 0;

    ComPtr<ID3D12DescriptorHeap> rtvDescHeap;
    ComPtr<ID3D12DescriptorHeap> dsvDescHeap;
    ComPtr<ID3D12DescriptorHeap> cbvSrvUavDescHeap;

    D3D_DRIVER_TYPE d3dDriverType = D3D_DRIVER_TYPE_HARDWARE;
    // TODO: Make this SRGB later.
    DXGI_FORMAT backBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
    DXGI_FORMAT depthStencilFormat = DXGI_FORMAT_D32_FLOAT;
    const uint32_t clientWidth = 600;
    const uint32_t clientHeight = 900;

    void FlushCommandQueue();

    D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentBackBufferHandle() const;
    D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentFrontBufferHandle() const;
    D3D12_CPU_DESCRIPTOR_HANDLE GetDepthStencilHandle() const;

    ME::Shader* shader = nullptr;
    ME::QuadDX* quad = nullptr;
    // Per pass constant buffer.
    ME::UploadBufferDX* perPassCB = nullptr;
    // Per object constant buffer.
    ME::UploadBufferDX* perObjCB = nullptr;
    // Instance buffer for sprite renderer.
    ME::UploadBufferDX* spriteInstanceBuffer = nullptr;
    // Texture atlas properties buffer.
    ME::UploadBufferDX* textureAtlasPropsBuffer = nullptr;

    uint32_t frameCounter = 0;

    ME::Camera* camera = nullptr;
    ME::Light* directionalLight = nullptr;
    ME::Light* ambientLight = nullptr;
    void CreateCameraAndLights();

    uint32_t cbvPerObjectCount = 16'000;
    uint32_t srvCount = 16;

    ME::TextureDX* texture1 = nullptr;

    ME::SpriteRendererInstanceData* spriteInstanceData = nullptr;
    uint32_t countX = 15;
    uint32_t countY = 30;
    uint32_t spriteInstanceCount = countX * countY;
    D3D12_GPU_DESCRIPTOR_HANDLE spriteInstanceBufferSrvHandle;
};

}  // namespace ME

#endif  // VG_WIN
