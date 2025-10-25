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
#include "texture_dx.h"
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

    D3D12_RESOURCE_DESC depthStencilDesc{};
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

    D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
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

    D3D12_DESCRIPTOR_HEAP_DESC cbvSrvUavDescHeapDesc{};
    // PerPass + some objects.
    cbvSrvUavDescHeapDesc.NumDescriptors = (1 + cbvPerObjectCount + srvCount);
    cbvSrvUavDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    cbvSrvUavDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    cbvSrvUavDescHeapDesc.NodeMask = 0;
    device->CreateDescriptorHeap(&cbvSrvUavDescHeapDesc, IID_PPV_ARGS(&cbvSrvUavDescHeap));

    rootSignature = RootSigDx::CreateRootSignature2D(device.Get());
    pso = PSODirectX::CreatePSO2D(device.Get(), "sprite.hlsl", rootSignature);
    // pso = PSODirectX::CreatePSO3D(device.Get(), "lit_alpha.hlsl", rootSignature);

    CreateCameraAndLights();

    // Do Initilization that need command list.
    directCmdListAlloc->Reset();
    commandList->Reset(directCmdListAlloc.Get(), pso);

    CD3DX12_RESOURCE_BARRIER depthBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
        depthStencilBuffer.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE);
    commandList->ResourceBarrier(1, &depthBarrier);

    shader = new Shader{"sprite.hlsl"};

    quad = new QuadDX{"quad", device.Get(), commandList.Get()};
    quad->CreateBuffers(device.Get(), commandList.Get());

    perPassCB = new UploadBufferDX(device.Get(), true, 1, sizeof(CBPerPass));
    D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
    cbvDesc.BufferLocation = perPassCB->GetResource()->GetGPUVirtualAddress();
    cbvDesc.SizeInBytes = perPassCB->GetElementSize();
    D3D12_CPU_DESCRIPTOR_HANDLE cbvHandle = cbvSrvUavDescHeap->GetCPUDescriptorHandleForHeapStart();
    cbvHandle.ptr += 0 * cbvSrvUavDescriptorSize;
    device->CreateConstantBufferView(&cbvDesc, cbvHandle);

    perObjCB = new UploadBufferDX(device.Get(), true, 1, sizeof(CBPerObject));
    D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc2 = {};
    cbvDesc2.BufferLocation = perObjCB->GetResource()->GetGPUVirtualAddress();
    cbvDesc2.SizeInBytes = perObjCB->GetElementSize();
    D3D12_CPU_DESCRIPTOR_HANDLE cbvHandle2 = cbvSrvUavDescHeap->GetCPUDescriptorHandleForHeapStart();
    cbvHandle2.ptr += 1 * cbvSrvUavDescriptorSize;
    device->CreateConstantBufferView(&cbvDesc2, cbvHandle2);

    texture1 = new TextureDX{"textures/world/poppy128.png", device.Get(), commandList.Get()};
    texture1->CreateBuffers(device.Get(), commandList.Get());

    texture2 = new TextureDX{"textures/world/torchflower128.png", device.Get(), commandList.Get()};
    texture2->CreateBuffers(device.Get(), commandList.Get());

    texture3 = new TextureDX{"textures/world/peony_top128.png", device.Get(), commandList.Get()};
    texture3->CreateBuffers(device.Get(), commandList.Get());

    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.Texture2D.MipLevels = -1;

    // texture 1 SRV
    D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = cbvSrvUavDescHeap->GetCPUDescriptorHandleForHeapStart();
    srvHandle.ptr += 2 * cbvSrvUavDescriptorSize;
    device->CreateShaderResourceView(texture1->GetTextureBuffer(), &srvDesc, srvHandle);
    D3D12_GPU_DESCRIPTOR_HANDLE cbvHandleTex = cbvSrvUavDescHeap->GetGPUDescriptorHandleForHeapStart();
    cbvHandleTex.ptr += 2 * cbvSrvUavDescriptorSize;
    texture1->srvHandle = cbvHandleTex;

    // texture 2 SRV
    srvHandle = cbvSrvUavDescHeap->GetCPUDescriptorHandleForHeapStart();
    srvHandle.ptr += 3 * cbvSrvUavDescriptorSize;
    device->CreateShaderResourceView(texture2->GetTextureBuffer(), &srvDesc, srvHandle);
    cbvHandleTex = cbvSrvUavDescHeap->GetGPUDescriptorHandleForHeapStart();
    cbvHandleTex.ptr += 3 * cbvSrvUavDescriptorSize;
    texture2->srvHandle = cbvHandleTex;

    // texture 3 SRV
    srvHandle = cbvSrvUavDescHeap->GetCPUDescriptorHandleForHeapStart();
    srvHandle.ptr += 4 * cbvSrvUavDescriptorSize;
    device->CreateShaderResourceView(texture3->GetTextureBuffer(), &srvDesc, srvHandle);
    cbvHandleTex = cbvSrvUavDescHeap->GetGPUDescriptorHandleForHeapStart();
    cbvHandleTex.ptr += 4 * cbvSrvUavDescriptorSize;
    texture3->srvHandle = cbvHandleTex;

    commandList->Close();
    ID3D12CommandList* cmdsLists[] = {commandList.Get()};
    commandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);
    FlushCommandQueue();

    quad->ReleaseUploadBuffers();
    texture1->ReleaseUploadBuffers();
    texture2->ReleaseUploadBuffers();
    texture3->ReleaseUploadBuffers();

    return true;
}

void ME::RendererDirectX::Draw() {
    directCmdListAlloc->Reset();
    commandList->Reset(directCmdListAlloc.Get(), pso);

    commandList->SetPipelineState(pso);
    commandList->SetGraphicsRootSignature(rootSignature);

    ID3D12DescriptorHeap* descriptorHeaps[] = {cbvSrvUavDescHeap.Get()};
    commandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

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

    ++frameCounter;
    float angle = frameCounter * 0.01f;
    ME::Vec16 viewMatrix = camera->GetViewMatrix().GetDataRowMajor();
    ME::Vec16 projectionMatrix = camera->GetProjectionMatrix().GetDataRowMajor();

    CBPerPass constantData{};
    constantData.viewMatrix = viewMatrix;
    constantData.projectionMatrix = projectionMatrix;
    constantData.ambientLightData = ambientLight->GetLightDataAmbient();
    constantData.directionalLightData = directionalLight->GetLightDataDirectional();
    perPassCB->CopyData(0, &constantData);

    ME::Transform modelTransform;
    modelTransform.SetScale(8.0f, 8.0f, 1.0f);
    modelTransform.SetRotation(0.0f, 0.0f, 0.0f);
    modelTransform.SetPosition(ME::Vec3(0.0f, 0.0f, 0.0f));
    ME::Vec16 modelMatrix = modelTransform.GetModelMatrix().GetDataRowMajor();

    CBPerObject objConstantData{};
    objConstantData.modelMatrix = modelMatrix;
    perObjCB->CopyData(0, &objConstantData);

    D3D12_GPU_DESCRIPTOR_HANDLE cbvHandlePerPass = cbvSrvUavDescHeap->GetGPUDescriptorHandleForHeapStart();
    commandList->SetGraphicsRootDescriptorTable(0, cbvHandlePerPass);
    D3D12_GPU_DESCRIPTOR_HANDLE cbvHandlePerObject = cbvSrvUavDescHeap->GetGPUDescriptorHandleForHeapStart();
    cbvHandlePerObject.ptr += 1 * cbvSrvUavDescriptorSize;
    commandList->SetGraphicsRootDescriptorTable(1, cbvHandlePerObject);

    int currentTexture = (frameCounter / 24) % 3;  // Change texture every 24 frames

    D3D12_GPU_DESCRIPTOR_HANDLE textureHandle;
    switch (currentTexture) {
        case 0:
            textureHandle = texture1->srvHandle;
            break;
        case 1:
            textureHandle = texture2->srvHandle;
            break;
        case 2:
            textureHandle = texture3->srvHandle;
            break;
        default:
            textureHandle = texture1->srvHandle;
            break;
    }
    commandList->SetGraphicsRootDescriptorTable(2, textureHandle);

    D3D12_VERTEX_BUFFER_VIEW vbView = quad->GetVertexBufferView();
    D3D12_INDEX_BUFFER_VIEW ibView = quad->GetIndexBufferView();
    commandList->IASetVertexBuffers(0, 1, &vbView);
    commandList->IASetIndexBuffer(&ibView);
    commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    commandList->DrawIndexedInstanced(quad->indexCount, 1, 0, 0, 0);

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
    camera->position = ME::Vec3(0.0f, 0.0f, -10.0f);
    camera->viewPosition = ME::Vec3(0.0f, 0.0f, 0.0f);
    camera->projectionType = ME::ProjectionType::Perspective;
    camera->fov = 90.0f;
    camera->aspectRatio = static_cast<float>(clientWidth) / static_cast<float>(clientHeight);

    ambientLight = new ME::Light();
    ambientLight->color = ME::Color::White();
    ambientLight->intensity = 0.1f;

    directionalLight = new ME::Light();
    directionalLight->direction = ME::Vec3(-4.0f, -10.0f, 4.0f).Normalised();
    directionalLight->color = ME::Color::White();
    directionalLight->intensity = 1.0f;
}

#endif  // VG_WIN
