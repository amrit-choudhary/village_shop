#ifdef VG_WIN

#include "utils_directx.h"

#include "../shared/vertex.h"
#include "src/math/vec2.h"

ID3D12Resource* ME::UtilsDirectX::CreateDefaultBufferResource(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList,
                                                              const void* initData, uint32_t byteSize,
                                                              ID3D12Resource** uploadBuffer) {
    ID3D12Resource* defaultBuffer = nullptr;
    CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_DEFAULT);

    return nullptr;
}

#endif  // VG_WIN
