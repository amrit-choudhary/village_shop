#if VG_WIN

#include "renderer_dx.h"

#include <cmath>

#include "../shared/camera.h"
#include "../shared/light.h"
#include "../shared/mesh.h"
#include "../shared/mesh_parser_obj.h"
#include "../shared/texture.h"
#include "d3dx12.h"
#include "mesh_dx.h"
#include "pso_dx.h"
#include "quad_dx.h"
#include "root_sig_dx.h"
#include "shader_dx.h"
#include "src/logging.h"
#include "src/math/transform.h"
#include "src/math/vec16.h"
#include "src/math/vec3.h"
#include "upload_buffer_dx.h"
#include "utils_dx.h"

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

    device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));

    rtvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    dsvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
    cbvSrvUavDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

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
    scDesc.Format = backBufferFormat;
    scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    scDesc.SampleDesc.Count = 1;

    factory->CreateSwapChainForHwnd(commandQueue.Get(), hWnd, &scDesc, nullptr, nullptr,
                                    swapChain.ReleaseAndGetAddressOf());

    D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc;
    rtvDescHeapDesc.NumDescriptors = swapChainBufferCount;
    rtvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    rtvDescHeapDesc.NodeMask = 0;
    device->CreateDescriptorHeap(&rtvDescHeapDesc, IID_PPV_ARGS(&rtvDescHeap));

    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(rtvDescHeap->GetCPUDescriptorHandleForHeapStart());
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
    device->CreateDescriptorHeap(&dsvDescHeapDesc, IID_PPV_ARGS(&dsvDescHeap));

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
                                   dsvDescHeap->GetCPUDescriptorHandleForHeapStart());

    screenViewport.TopLeftX = 0;
    screenViewport.TopLeftY = 0;
    screenViewport.Width = static_cast<float>(clientWidth);
    screenViewport.Height = static_cast<float>(clientHeight);
    screenViewport.MinDepth = 0.0f;
    screenViewport.MaxDepth = 1.0f;

    scissorRect = {0, 0, static_cast<LONG>(clientWidth), static_cast<LONG>(clientHeight)};

    D3D12_DESCRIPTOR_HEAP_DESC cbvSrvUavDescHeapDesc;
    // PerPass + some objects.
    cbvSrvUavDescHeapDesc.NumDescriptors = (1 + cbvPerObjectCount);
    cbvSrvUavDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    cbvSrvUavDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    cbvSrvUavDescHeapDesc.NodeMask = 0;
    device->CreateDescriptorHeap(&cbvSrvUavDescHeapDesc, IID_PPV_ARGS(&cbvSrvUavDescHeap));

    rootSignature = RootSigDx::CreateRootSignature2D(device.Get());
    // pso = PSODirectX::CreatePSO2D(device.Get(), "sprite.hlsl", rootSignature);
    pso = PSODirectX::CreatePSO3D(device.Get(), "unlit.hlsl", rootSignature);

    CreateCameraAndLights();

    // Do Initilization that need command list.
    directCmdListAlloc->Reset();
    commandList->Reset(directCmdListAlloc.Get(), pso);

    CD3DX12_RESOURCE_BARRIER depthBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
        depthStencilBuffer.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE);
    commandList->ResourceBarrier(1, &depthBarrier);

    shader = new Shader{"sprite.hlsl"};

    quad = new QuadDirectX{"quad", device.Get(), commandList.Get()};
    quad->CreateBuffers(device.Get(), commandList.Get());

    mesh = new MeshDx{"meshes/cars/race.obj", device.Get(), commandList.Get()};
    mesh->CreateBuffers(device.Get(), commandList.Get());

    constantBuffer = new UploadBufferDX(device.Get(), true, (1 + cbvPerObjectCount), sizeof(CBPerPass));
    for (int objIdx = 0; objIdx < (1 + cbvPerObjectCount); ++objIdx) {
        D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
        cbvDesc.BufferLocation =
            constantBuffer->GetResource()->GetGPUVirtualAddress() + objIdx * constantBuffer->GetElementSize();
        cbvDesc.SizeInBytes = constantBuffer->GetElementSize();
        D3D12_CPU_DESCRIPTOR_HANDLE cbvHandle = cbvSrvUavDescHeap->GetCPUDescriptorHandleForHeapStart();
        cbvHandle.ptr += objIdx * cbvSrvUavDescriptorSize;
        device->CreateConstantBufferView(&cbvDesc, cbvHandle);
    }

    commandList->Close();
    ID3D12CommandList* cmdsLists[] = {commandList.Get()};
    commandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);
    FlushCommandQueue();

    quad->ReleaseUploadBuffers();
    mesh->ReleaseUploadBuffers();

    return true;
}

void ME::RendererDirectX::Draw() {
    directCmdListAlloc->Reset();
    commandList->Reset(directCmdListAlloc.Get(), pso);

    commandList->RSSetViewports(1, &screenViewport);
    commandList->RSSetScissorRects(1, &scissorRect);

    CD3DX12_RESOURCE_BARRIER presentToRenderTargetBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
        swapChainBuffers[currentBackBuffer].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
    commandList->ResourceBarrier(1, &presentToRenderTargetBarrier);

    float clearColor[4] = {0.01f, 0.01f, 0.01f, 1.0f};

    commandList->ClearRenderTargetView(GetCurrentBackBufferHandle(), &clearColor[0], 0, nullptr);
    commandList->ClearDepthStencilView(GetDepthStencilHandle(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f,
                                       0, 0, nullptr);

    D3D12_CPU_DESCRIPTOR_HANDLE backBufferHandle = GetCurrentBackBufferHandle();
    D3D12_CPU_DESCRIPTOR_HANDLE depthStencilHandle = GetDepthStencilHandle();
    commandList->OMSetRenderTargets(1, &backBufferHandle, true, &depthStencilHandle);
    ID3D12DescriptorHeap* descriptorHeaps[] = {cbvSrvUavDescHeap.Get()};
    commandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

    // Actual drawing.

    // 2D Drawing
    // commandList->SetPipelineState(pso);
    // commandList->SetGraphicsRootSignature(rootSignature);
    // commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    // commandList->SetGraphicsRootDescriptorTable(0, cbvSrvUavDescHeap->GetGPUDescriptorHandleForHeapStart());

    // D3D12_VERTEX_BUFFER_VIEW vbView = quad->GetVertexBufferView();
    // D3D12_INDEX_BUFFER_VIEW ibView = quad->GetIndexBufferView();
    // commandList->IASetVertexBuffers(0, 1, &vbView);
    // commandList->IASetIndexBuffer(&ibView);
    // commandList->DrawIndexedInstanced(quad->indexCount, 1, 0, 0, 0);

    // 3D Drawing
    commandList->SetPipelineState(pso);
    commandList->SetGraphicsRootSignature(rootSignature);
    commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    D3D12_VERTEX_BUFFER_VIEW vbView = mesh->GetVertexBufferView();
    D3D12_INDEX_BUFFER_VIEW ibView = mesh->GetIndexBufferView();
    commandList->IASetVertexBuffers(0, 1, &vbView);
    commandList->IASetIndexBuffer(&ibView);

    ++frameCounter;
    float angle = frameCounter * 0.01f;
    ME::Vec16 viewMatrix = camera->GetViewMatrix().GetDataRowMajor();
    ME::Vec16 projectionMatrix = camera->GetProjectionMatrix().GetDataRowMajor();

    CBPerPass constantData{};
    constantData.viewMatrix = viewMatrix;
    constantData.projectionMatrix = projectionMatrix;
    constantData.ambientLightData = ambientLight->GetLightDataAmbient();
    constantData.directionalLightData = directionalLight->GetLightDataDirectional();
    constantBuffer->CopyData(0, &constantData);

    D3D12_GPU_DESCRIPTOR_HANDLE cbvHandle = cbvSrvUavDescHeap->GetGPUDescriptorHandleForHeapStart();
    commandList->SetGraphicsRootDescriptorTable(0, cbvHandle);

    // Starting from 1 since 0 is reserved for per-pass data.
    int objIdx = 1;
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            for (int k = 0; k < 5; ++k) {
                float dispX = (i - 2) * 4.0f;
                float dispY = (j - 2) * 4.0f;
                float dispZ = (k - 2) * 4.0f;

                ME::Transform modelTransform;
                modelTransform.SetScale(1.0f, 1.0f, 1.0f);
                modelTransform.SetRotation(0, angle, 0.0f);
                modelTransform.SetPosition(ME::Vec3(dispX, dispY, dispZ));
                ME::Vec16 modelMatrix = modelTransform.GetModelMatrix().GetDataRowMajor();

                CBPerObject constantData{};
                constantData.modelMatrix = modelMatrix;
                constantBuffer->CopyData(objIdx, &constantData);

                D3D12_GPU_DESCRIPTOR_HANDLE cbvHandle = cbvSrvUavDescHeap->GetGPUDescriptorHandleForHeapStart();
                cbvHandle.ptr += objIdx * cbvSrvUavDescriptorSize;
                commandList->SetGraphicsRootDescriptorTable(1, cbvHandle);

                commandList->DrawIndexedInstanced(mesh->indexCount, 1, 0, 0, 0);
                ++objIdx;
            }
        }
    }

    // End drawing.

    CD3DX12_RESOURCE_BARRIER renderTargetToPresentBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
        swapChainBuffers[currentBackBuffer].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
    commandList->ResourceBarrier(1, &renderTargetToPresentBarrier);

    commandList->Close();

    ID3D12CommandList* cmdsLists[] = {commandList.Get()};
    commandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

    swapChain->Present(0, 0);
    currentBackBuffer = (currentBackBuffer + 1) % swapChainBufferCount;

    FlushCommandQueue();
}

void ME::RendererDirectX::FlushCommandQueue() {
    ++currentFence;
    commandQueue->Signal(fence.Get(), currentFence);
    if (fence->GetCompletedValue() < currentFence) {
        HANDLE fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
        fence->SetEventOnCompletion(currentFence, fenceEvent);
        WaitForSingleObject(fenceEvent, INFINITE);
        CloseHandle(fenceEvent);
    }
}

D3D12_CPU_DESCRIPTOR_HANDLE ME::RendererDirectX::GetCurrentBackBufferHandle() const {
    return CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvDescHeap->GetCPUDescriptorHandleForHeapStart(), currentBackBuffer,
                                         rtvDescriptorSize);
}

D3D12_CPU_DESCRIPTOR_HANDLE ME::RendererDirectX::GetCurrentFrontBufferHandle() const {
    uint8_t currentFrontBuffer = (currentBackBuffer + 1) % swapChainBufferCount;
    return CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvDescHeap->GetCPUDescriptorHandleForHeapStart(), currentFrontBuffer,
                                         rtvDescriptorSize);
}

D3D12_CPU_DESCRIPTOR_HANDLE ME::RendererDirectX::GetDepthStencilHandle() const {
    return dsvDescHeap->GetCPUDescriptorHandleForHeapStart();
}

void ME::RendererDirectX::CreateCameraAndLights() {
    camera = new ME::Camera();
    camera->position = ME::Vec3(10.0f, 10.0f, -10.0f);
    camera->viewPosition = ME::Vec3(0.0f, 0.0f, 0.0f);
    camera->projectionType = ME::ProjectionType::Perspective;
    camera->fov = 90.0f;
    camera->aspectRatio = 1.33f;

    ambientLight = new ME::Light();
    ambientLight->color = ME::Color::White();
    ambientLight->intensity = 0.04f;

    directionalLight = new ME::Light();
    directionalLight->direction = ME::Vec3(-4.0f, -4.0f, 4.0f).Normalised();
    directionalLight->color = ME::Color::White();
    directionalLight->intensity = 1.0f;
}

#endif  // VG_WIN
