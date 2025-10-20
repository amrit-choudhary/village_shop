#pragma once
#ifdef VG_WIN

/**
 * DirectX's specific upload buffer class.
 * Used for uploading data to the GPU like constant buffers.
 * This creates an upload heap buffer and maps it to a CPU pointer to be able to write to it.
 */

#include <d3d12.h>
#include <stdint.h>

namespace ME {

class UploadBufferDX {
   public:
    UploadBufferDX(ID3D12Device* device, bool isConstantBuffer, uint32_t elementCount, uint32_t elementSize);
    ~UploadBufferDX();

    UploadBufferDX(const UploadBufferDX&) = delete;
    UploadBufferDX& operator=(const UploadBufferDX&) = delete;

    ID3D12Resource* GetResource() const;
    uint8_t* GetMappedData() const;
    uint32_t GetElementSize() const;
    void CopyData(uint32_t elementIndex, const void* data);

   private:
    bool isConstantBuffer = false;
    uint32_t elementCount = 0;
    uint32_t elementSize = 0;
    ID3D12Resource* uploadBuffer = nullptr;
    uint8_t* mappedData = nullptr;
};

}  // namespace ME

#endif  // VG_WIN
