#ifdef VG_WIN

#include "utils_directx.h"

#include "../shared/vertex.h"
#include "src/math/vec2.h"

ID3D12Resource* ME::UtilsDirectX::CreateDefaultBufferResource(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList,
                                                              const void* initData, uint32_t byteSize,
                                                              ID3D12Resource** uploadBuffer) {
    if (!device || !cmdList || !initData || !uploadBuffer) return nullptr;

    ID3D12Resource* defaultBuffer = nullptr;
    *uploadBuffer = nullptr;

    CD3DX12_HEAP_PROPERTIES defaultHeapProps(D3D12_HEAP_TYPE_DEFAULT);
    CD3DX12_HEAP_PROPERTIES uploadHeapProps(D3D12_HEAP_TYPE_UPLOAD);
    CD3DX12_RESOURCE_DESC bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(byteSize);

    HRESULT hr = device->CreateCommittedResource(&defaultHeapProps, D3D12_HEAP_FLAG_NONE, &bufferDesc,
                                                 D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&defaultBuffer));

    if (FAILED(hr)) {
        return nullptr;
    }

    hr = device->CreateCommittedResource(&uploadHeapProps, D3D12_HEAP_FLAG_NONE, &bufferDesc,
                                         D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(uploadBuffer));

    if (FAILED(hr)) {
        defaultBuffer->Release();
        return nullptr;
    }

    D3D12_SUBRESOURCE_DATA subResourceData = {};
    subResourceData.pData = initData;
    subResourceData.RowPitch = byteSize;
    subResourceData.SlicePitch = subResourceData.RowPitch;

    UpdateSubresources<1>(cmdList, defaultBuffer, *uploadBuffer, 0, 0, 1, &subResourceData);

    CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
        defaultBuffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ);
    cmdList->ResourceBarrier(1, &barrier);

    // Note: caller must execute the command list and ensure GPU finishes the copy before
    // releasing the uploadBuffer (or keep uploadBuffer alive until the fence signals).
    return defaultBuffer;
}

ID3D12RootSignature* ME::UtilsDirectX::CreateSimpleRootSignature(ID3D12Device* device) {
    if (!device) return nullptr;

    D3D12_ROOT_SIGNATURE_DESC desc = {};
    desc.NumParameters = 0;
    desc.pParameters = nullptr;
    desc.NumStaticSamplers = 0;
    desc.pStaticSamplers = nullptr;
    desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

    ID3DBlob* serialized;
    ID3DBlob* error;
    HRESULT hr = D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION_1, &serialized, &error);
    if (FAILED(hr)) {
        if (error) OutputDebugStringA(static_cast<const char*>(error->GetBufferPointer()));
        return nullptr;
    }

    ID3D12RootSignature* rootSig;
    hr = device->CreateRootSignature(0, serialized->GetBufferPointer(), serialized->GetBufferSize(),
                                     IID_PPV_ARGS(&rootSig));
    if (FAILED(hr)) return nullptr;

    return rootSig;
}

#endif  // VG_WIN
