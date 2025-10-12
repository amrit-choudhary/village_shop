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
    uint32_t currentBackBuffer = 0;
    ComPtr<ID3D12Resource> swapChainBuffers[swapChainBufferCount];
    ComPtr<ID3D12Resource> depthStencilBuffer;

    ComPtr<ID3D12DescriptorHeap> rtvHeap;
    ComPtr<ID3D12DescriptorHeap> dsvHeap;

    D3D12_VIEWPORT screenViewport;
    D3D12_RECT scissorRect;

    uint32_t rtvDescriptorSize = 0;
    uint32_t dsvDescriptorSize = 0;
    uint32_t cbvSrvUavDescriptorSize = 0;

    ComPtr<ID3D12DescriptorHeap> rtvDescriptorHeap;
    ComPtr<ID3D12DescriptorHeap> dsvDescriptorHeap;

    D3D_DRIVER_TYPE d3dDriverType = D3D_DRIVER_TYPE_HARDWARE;
    DXGI_FORMAT backBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
    DXGI_FORMAT depthStencilFormat = DXGI_FORMAT_D32_FLOAT;
    uint32_t clientWidth = 1024;
    uint32_t clientHeight = 768;
};

}  // namespace ME

#endif  // VG_WIN
