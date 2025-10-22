#ifdef VG_WIN

#include "root_sig_dx.h"

#include "d3dx12.h"

ID3D12RootSignature* ME::RootSigDx::CreateRootSignature2D(ID3D12Device* device) {
    if (!device) return nullptr;

    CD3DX12_ROOT_PARAMETER rootParameters[2];

    // Per Pass at register(b0)
    CD3DX12_DESCRIPTOR_RANGE cbvRangePass;
    cbvRangePass.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
    rootParameters[0].InitAsDescriptorTable(1, &cbvRangePass);

    // Per Object at register(b1)
    CD3DX12_DESCRIPTOR_RANGE cbvRangeObject;
    cbvRangeObject.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 1);
    rootParameters[1].InitAsDescriptorTable(1, &cbvRangeObject);

    D3D12_ROOT_SIGNATURE_DESC desc = {};
    desc.NumParameters = _countof(rootParameters);
    desc.pParameters = rootParameters;
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
