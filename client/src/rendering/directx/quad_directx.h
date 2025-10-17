#pragma once
#ifdef VG_WIN

/**
 * DirectX's specific quad class.
 */

#include <d3d12.h>

#include "../shared/quad.h"

namespace ME {

class QuadDirectX : public Quad {
   public:
    QuadDirectX(const char* path, ID3D12Device* device, ID3D12GraphicsCommandList* cmdList);
    ~QuadDirectX() override;

    ID3D12Resource* vertexBuffer = nullptr;
    ID3D12Resource* indexBuffer = nullptr;

    ID3D12Resource* vertexBufferUpload = nullptr;
    ID3D12Resource* indexBufferUpload = nullptr;
};

}  // namespace ME

#endif  // VG_WIN
