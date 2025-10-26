#pragma once
#ifdef VG_WIN

/**
 * DirectX's specific root signature class.
 * Contains methods to create root signatures for shaders.
 */

#include <d3d12.h>

// Forward declaration
class CD3DX12_STATIC_SAMPLER_DESC;

namespace ME {

class RootSigDx {
   public:
    RootSigDx() = default;
    ~RootSigDx() = default;

    /** Create root signature for 2D rendering. */
    static ID3D12RootSignature* CreateRootSignature2D(ID3D12Device* device);

    /** Create root signature for 2D rendering with Instancing. */
    static ID3D12RootSignature* CreateRootSignature2DInstanced(ID3D12Device* device);

    /** Create root signature for 3D rendering. */
    static ID3D12RootSignature* CreateRootSignature3D(ID3D12Device* device);

   private:
    // Create and return 6 common static sampler descriptors.
    static CD3DX12_STATIC_SAMPLER_DESC* GetStaticSamplerDescriptors();
};

}  // namespace ME

#endif  // VG_WIN
