#ifdef VG_WIN

#include "texture_dx.h"

#include <cstring>

ME::TextureDX::TextureDX() : Texture() {}

ME::TextureDX::~TextureDX() {}

ME::TextureDX::TextureDX(const char* path, ID3D12Device* device, ID3D12GraphicsCommandList* cmdList) {
    Texture::Load(path);
}

ID3D12Resource* ME::TextureDX::GetTextureBuffer() const {
    return textureBuffer;
}

void ME::TextureDX::CreateBuffers(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList) {
    CD3DX12_HEAP_PROPERTIES defaultHeapProps(D3D12_HEAP_TYPE_DEFAULT);
    CD3DX12_HEAP_PROPERTIES uploadHeapProps(D3D12_HEAP_TYPE_UPLOAD);

    // Calculate padded row pitch because every row must be aligned to D3D12_TEXTURE_DATA_PITCH_ALIGNMENT (256 bytes).
    UINT64 rowPitch =
        (width * channels + D3D12_TEXTURE_DATA_PITCH_ALIGNMENT - 1) & ~(D3D12_TEXTURE_DATA_PITCH_ALIGNMENT - 1);
    UINT64 uploadBufferSize = rowPitch * height;

    CD3DX12_RESOURCE_DESC bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize);

    // Create the texture buffer resource in the default heap.
    CD3DX12_RESOURCE_DESC textureDesc = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_R8G8B8A8_UNORM, width, height, 1, 1);
    device->CreateCommittedResource(&defaultHeapProps, D3D12_HEAP_FLAG_NONE, &textureDesc,
                                    D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&textureBuffer));

    // Create the upload buffer.
    device->CreateCommittedResource(&uploadHeapProps, D3D12_HEAP_FLAG_NONE, &bufferDesc,
                                    D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&uploadBuffer));

    // Copy data to the upload buffer with row padding.
    uint8_t* mappedData = nullptr;
    CD3DX12_RANGE readRange(0, 0);
    uploadBuffer->Map(0, &readRange, reinterpret_cast<void**>(&mappedData));
    for (uint32_t y = 0; y < height; ++y) {
        std::memcpy(mappedData + y * rowPitch, data + y * width * channels, width * channels);
        std::memset(mappedData + y * rowPitch + width * channels, 0, rowPitch - width * channels);
    }
    uploadBuffer->Unmap(0, nullptr);

    // Prepare subresource data
    D3D12_SUBRESOURCE_DATA textureData = {};
    textureData.pData = mappedData;
    textureData.RowPitch = rowPitch;
    textureData.SlicePitch = rowPitch * height;

    UpdateSubresources<1>(cmdList, textureBuffer, uploadBuffer, 0, 0, 1, &textureData);

    // Transition the texture buffer to PIXEL_SHADER_RESOURCE state.
    CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
        textureBuffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
    cmdList->ResourceBarrier(1, &barrier);
}

void ME::TextureDX::ReleaseUploadBuffers() {
    if (uploadBuffer) {
        uploadBuffer->Release();
        uploadBuffer = nullptr;
    }
}

void ME::TextureDX::CreateGPUHandle(ID3D12Device* device, ID3D12DescriptorHeap* descriptorHeap, uint32_t index) {
    uint32_t handleSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

    D3D12_SHADER_RESOURCE_VIEW_DESC handleDesc{};
    handleDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    handleDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    handleDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    handleDesc.Texture2D.MostDetailedMip = 0;
    handleDesc.Texture2D.MipLevels = -1;

    D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
    cpuHandle.ptr += index * handleSize;
    device->CreateShaderResourceView(textureBuffer, &handleDesc, cpuHandle);

    D3D12_GPU_DESCRIPTOR_HANDLE tempGPUHandle = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
    tempGPUHandle.ptr += index * handleSize;
    gpuHandle = tempGPUHandle;
}

#endif  // VG_WIN
