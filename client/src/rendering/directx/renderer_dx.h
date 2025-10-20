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
class Shader;
class QuadDirectX;
class UploadBufferDX;
class MeshDx;

/**
 * Renderer class for DirectX 12 on Windows.
 */
class RendererDirectX : public PlatformRenderer {
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
    bool InitDirectX(HWND hwnd);

    void Draw();

   private:
    HWND hWnd;
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
    const uint32_t clientWidth = 768;
    const uint32_t clientHeight = 1024;

    void FlushCommandQueue();

    D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentBackBufferHandle() const;
    D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentFrontBufferHandle() const;
    D3D12_CPU_DESCRIPTOR_HANDLE GetDepthStencilHandle() const;

    ME::Shader* shader = nullptr;
    ME::QuadDirectX* quad = nullptr;
    ME::UploadBufferDX* constantBuffer = nullptr;
    ME::MeshDx* mesh = nullptr;

    uint32_t frameCounter = 0;
};

}  // namespace ME

#endif  // VG_WIN
