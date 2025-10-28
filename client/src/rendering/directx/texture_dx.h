#pragma once
#ifdef VG_WIN

/**
 * DirectX's specific texture class.
 */

#include "../shared/texture.h"
#include "d3dx12.h"

namespace ME {

/**
 * DirectX's specific texture class.
 * This class is used to load and manage textures using DirectX.
 */
class TextureDX : public Texture {
   public:
    TextureDX();
    ~TextureDX() override;
    /** 1. Loads a PNG texture from file.
     * 2. Create a DirectX texture buffer.
     * 3. Upload the data to the GPU.
     */
    TextureDX(const char* path, ID3D12Device* device, ID3D12GraphicsCommandList* cmdList);

    ID3D12Resource* GetTextureBuffer() const;

    /** Create the texture buffer on GPU.*/
    void CreateBuffers(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList);
    /** Release the upload buffers after data has been copied to GPU. */
    void ReleaseUploadBuffers();

    /** Creates a GPU handle for the texture. This is used for binding the texture to the pipeline. */
    void CreateGPUHandle(ID3D12Device* device, ID3D12DescriptorHeap* descriptorHeap, uint32_t index);

    D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle{};

   private:
    ID3D12Resource* textureBuffer = nullptr;
    ID3D12Resource* uploadBuffer = nullptr;
};

}  // namespace ME

#endif  // VG_WIN
