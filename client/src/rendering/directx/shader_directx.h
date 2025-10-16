#pragma once
#ifdef VG_WIN

/**
 * Shader class for DirectX
 * This will load a hlsl shader from a file and compile it.
 * It will also provide access to the vertex and pixel functions.
 */

#include <d3d12.h>
#include <wrl.h>

#include "../shared/renderer.h"

using Microsoft::WRL::ComPtr;

namespace ME {

class Shader {
   public:
    Shader(const char* shaderName);
    ~Shader();

    void Bind() const;
    void Unbind() const;

   private:
    ComPtr<ID3DBlob> vsBlob = nullptr;
    ComPtr<ID3DBlob> psBlob = nullptr;

    void CompileShader(const char* shaderName);
};

}  // namespace ME

#endif  // VG_WIN
