#ifdef VG_WIN

#include "shader_dx.h"

#include <D3Dcompiler.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <src/misc/utils.h>

#include <string>

ME::Shader::Shader(const char* shaderName) {
    CompileShader(shaderName);
}

ME::Shader::~Shader() {
    if (vsBlob) {
        vsBlob->Release();
        vsBlob = nullptr;
    }
    if (psBlob) {
        psBlob->Release();
        psBlob = nullptr;
    }
}

void ME::Shader::CompileShader(const char* shaderName) {
    std::string filePath = ME::Utils::GetResourcesPath() + "shaders/hlsl/" + shaderName;
    std::wstring wFilePath(filePath.begin(), filePath.end());

    HRESULT hr = S_OK;
    ID3DBlob* errorBlob = nullptr;

    // Compile Vertex Shader
    hr = D3DCompileFromFile(wFilePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VS", "vs_5_0", 0, 0,
                            &vsBlob, &errorBlob);
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to compile vertex shader: " + filePath);
    }

    // Compile Pixel Shader
    hr = D3DCompileFromFile(wFilePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PS", "ps_5_0", 0, 0,
                            &psBlob, &errorBlob);
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to compile pixel shader: " + filePath);
    }

    if (errorBlob) {
        errorBlob->Release();
    }
}

ID3DBlob* ME::Shader::GetVSBlob() const {
    return vsBlob;
}
ID3DBlob* ME::Shader::GetPSBlob() const {
    return psBlob;
}

#endif  // VG_WIN
