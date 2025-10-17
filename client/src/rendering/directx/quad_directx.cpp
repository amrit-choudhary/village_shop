#ifdef VG_WIN

#include "quad_directx.h"

ME::QuadDirectX::QuadDirectX(const char* path, ID3D12Device* device, ID3D12GraphicsCommandList* cmdList) : Quad() {}

ME::QuadDirectX::~QuadDirectX() {
    if (vertexBuffer) {
        vertexBuffer->Release();
        vertexBuffer = nullptr;
    }
    if (indexBuffer) {
        indexBuffer->Release();
        indexBuffer = nullptr;
    }
    if (vertexBufferUpload) {
        vertexBufferUpload->Release();
        vertexBufferUpload = nullptr;
    }
    if (indexBufferUpload) {
        indexBufferUpload->Release();
        indexBufferUpload = nullptr;
    }
}

#endif
