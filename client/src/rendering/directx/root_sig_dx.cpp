#ifdef VG_WIN

#include "root_sig_dx.h"

#include "d3dx12.h"

ID3D12RootSignature* ME::RootSigDx::CreateRootSignature2D(ID3D12Device* device) {
    if (!device) return nullptr;

    CD3DX12_ROOT_PARAMETER rootParameters[3];

    // Per Pass at register(b0)
    CD3DX12_DESCRIPTOR_RANGE cbvRangePass;
    cbvRangePass.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
    rootParameters[0].InitAsDescriptorTable(1, &cbvRangePass);

    // Per Object at register(b1)
    CD3DX12_DESCRIPTOR_RANGE cbvRangeObject;
    cbvRangeObject.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 1);
    rootParameters[1].InitAsDescriptorTable(1, &cbvRangeObject);

    // Texture at register(t0)
    CD3DX12_DESCRIPTOR_RANGE srvRange;
    srvRange.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
    rootParameters[2].InitAsDescriptorTable(1, &srvRange);

    // Add a static sampler at register(s0)
    CD3DX12_STATIC_SAMPLER_DESC samplerDesc(0,  // register s0
                                            D3D12_FILTER_MIN_MAG_MIP_POINT, D3D12_TEXTURE_ADDRESS_MODE_WRAP,
                                            D3D12_TEXTURE_ADDRESS_MODE_WRAP, D3D12_TEXTURE_ADDRESS_MODE_WRAP,
                                            0.0f,  // mipLODBias
                                            16,    // maxAnisotropy
                                            D3D12_COMPARISON_FUNC_ALWAYS, D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK,
                                            0.0f,              // minLOD
                                            D3D12_FLOAT32_MAX  // maxLOD
    );

    D3D12_ROOT_SIGNATURE_DESC desc = {};
    desc.NumParameters = _countof(rootParameters);
    desc.pParameters = rootParameters;
    desc.NumStaticSamplers = 1;
    desc.pStaticSamplers = &samplerDesc;
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
