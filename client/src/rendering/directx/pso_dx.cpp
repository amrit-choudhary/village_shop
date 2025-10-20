#ifdef VG_WIN
#include "pso_dx.h"

#include "d3dx12.h"

ME::PSODirectX::PSODirectX() {}

ME::PSODirectX::~PSODirectX() {}

void ME::PSODirectX::CreatePSO(ID3D12Device* device, const char* shaderName) {}

ID3D12PipelineState* ME::PSODirectX::CreatePSO2D(ID3D12Device* device, const char* shaderName,
                                                 ID3D12RootSignature* rootSignature) {
    ID3D12PipelineState* pso = nullptr;
    Shader shader(shaderName);

    D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
        {"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
        {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
    };

    CD3DX12_RASTERIZER_DESC rasterizerDesc(D3D12_DEFAULT);
    rasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;  // Disable culling for 2D

    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
    ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
    psoDesc.InputLayout = {inputLayout, _countof(inputLayout)};
    psoDesc.pRootSignature = rootSignature;
    psoDesc.VS = {shader.GetVSBlob()->GetBufferPointer(), shader.GetVSBlob()->GetBufferSize()};
    psoDesc.PS = {shader.GetPSBlob()->GetBufferPointer(), shader.GetPSBlob()->GetBufferSize()};
    psoDesc.RasterizerState = rasterizerDesc;
    psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
    psoDesc.SampleMask = UINT_MAX;
    psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    psoDesc.NumRenderTargets = 1;
    psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
    psoDesc.SampleDesc.Count = 1;
    psoDesc.SampleDesc.Quality = 0;
    device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pso));
    return pso;
}

#endif  // VG_WIN
