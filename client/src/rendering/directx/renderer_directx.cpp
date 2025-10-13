#if VG_WIN

#include "renderer_directx.h"

#include <cmath>

#include "../shared/camera.h"
#include "../shared/color.h"
#include "../shared/image_loader_png.h"
#include "../shared/light.h"
#include "../shared/mesh.h"
#include "../shared/mesh_parser_obj.h"
#include "../shared/texture.h"
#include "d3dx12.h"
#include "src/logging.h"
#include "src/math/transform.h"
#include "src/math/vec16.h"
#include "src/math/vec3.h"

void ME::RendererDirectX::Init() {
    ME::Log("RendererDirectX::Init");
}

void ME::RendererDirectX::SetScene(ME::Scene* gameScene) {
    // Delete the old scene if it exists.
    // if (scene != nullptr) {
    //     delete scene;
    // }
    // Create a new scene with the provided game scene.
    // scene = new ME::SceneMetal(device, commandQueue, gameScene);
}

void ME::RendererDirectX::Update() {
    // if (scene == nullptr) {
    //    return;
    // }
}

void ME::RendererDirectX::End() {}

bool ME::RendererDirectX::InitDirectX(HWND currenthWnd) {
#ifdef _DEBUG
    // Enable the D3D12 debug layer.
    ComPtr<ID3D12Debug> debugController;
    D3D12GetDebugInterface(IID_PPV_ARGS(&debugController));
    debugController->EnableDebugLayer();
#endif

    hWnd = currenthWnd;

    if (FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&factory)))) {
        MessageBoxW(hWnd, L"Failed to create DXGI Factory", L"Error", MB_OK);
        return false;
    }

    if (FAILED(D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device)))) {
        MessageBoxW(hWnd, L"Failed to create D3D12 Device", L"Error", MB_OK);
        return false;
    }

    D3D12_COMMAND_QUEUE_DESC cqDesc = {};
    cqDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    cqDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

    device->CreateCommandQueue(&cqDesc, IID_PPV_ARGS(&commandQueue));

    device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&directCmdListAlloc));
    device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, directCmdListAlloc.Get(), nullptr,
                              IID_PPV_ARGS(&commandList));
    commandList->Close();

    DXGI_SWAP_CHAIN_DESC1 scDesc = {};
    scDesc.BufferCount = 2;
    scDesc.Width = clientWidth;
    scDesc.Height = clientHeight;
    scDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  // This does not support sRGB. Render view format is set as sRGB.
    scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    scDesc.SampleDesc.Count = 1;

    factory->CreateSwapChainForHwnd(commandQueue.Get(), hWnd, &scDesc, nullptr, nullptr,
                                    swapChain.ReleaseAndGetAddressOf());

    device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));

    rtvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    dsvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
    cbvSrvUavDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

    D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc;
    rtvDescHeapDesc.NumDescriptors = swapChainBufferCount;
    rtvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    rtvDescHeapDesc.NodeMask = 0;
    device->CreateDescriptorHeap(&rtvDescHeapDesc, IID_PPV_ARGS(&rtvDescriptorHeap));

    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
    for (size_t i = 0; i < swapChainBufferCount; ++i) {
        swapChain->GetBuffer(i, IID_PPV_ARGS(&swapChainBuffers[i]));
        device->CreateRenderTargetView(swapChainBuffers[i].Get(), nullptr, rtvHeapHandle);
        rtvHeapHandle.Offset(1, rtvDescriptorSize);
    }

    D3D12_DESCRIPTOR_HEAP_DESC dsvDescHeapDesc;
    dsvDescHeapDesc.NumDescriptors = 1;
    dsvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    dsvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    dsvDescHeapDesc.NodeMask = 0;
    device->CreateDescriptorHeap(&dsvDescHeapDesc, IID_PPV_ARGS(&dsvDescriptorHeap));

    D3D12_RESOURCE_DESC depthStencilDesc = {};
    depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    depthStencilDesc.Alignment = 0;
    depthStencilDesc.Width = clientWidth;
    depthStencilDesc.Height = clientHeight;
    depthStencilDesc.DepthOrArraySize = 1;
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.Format = DXGI_FORMAT_D32_FLOAT;
    depthStencilDesc.SampleDesc.Count = 1;
    depthStencilDesc.SampleDesc.Quality = 0;
    depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

    D3D12_CLEAR_VALUE clearValue;
    clearValue.Format = DXGI_FORMAT_D32_FLOAT;
    clearValue.DepthStencil.Depth = 1.0f;
    clearValue.DepthStencil.Stencil = 0;
    CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_DEFAULT);
    device->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &depthStencilDesc, D3D12_RESOURCE_STATE_COMMON,
                                    &clearValue, IID_PPV_ARGS(&depthStencilBuffer));

    D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
    dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
    dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
    dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
    dsvDesc.Texture2D.MipSlice = 0;

    device->CreateDepthStencilView(depthStencilBuffer.Get(), &dsvDesc,
                                   dsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

    CD3DX12_RESOURCE_BARRIER depthStencilBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
        depthStencilBuffer.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE);
    commandList->ResourceBarrier(1, &depthStencilBarrier);

    screenViewport.TopLeftX = 0;
    screenViewport.TopLeftY = 0;
    screenViewport.Width = static_cast<float>(clientWidth);
    screenViewport.Height = static_cast<float>(clientHeight);
    screenViewport.MinDepth = 0.0f;
    screenViewport.MaxDepth = 1.0f;

    scissorRect = {0, 0, static_cast<LONG>(clientWidth), static_cast<LONG>(clientHeight)};

    return true;
}

void ME::RendererDirectX::Draw() {}

#endif  // VG_WIN