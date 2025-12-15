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
    std::string shaderBaseName{shaderName};
    size_t pos = shaderBaseName.find_last_of('.');
    // Remove extension if present.
    if (pos != std::string::npos) {
        shaderBaseName = shaderBaseName.substr(0, pos);
    }

    std::string filePathVS = ME::Utils::GetResourcesPath() + "shaders/hlsl/" + shaderBaseName + "_vs.cso";
    std::string filePathPS = ME::Utils::GetResourcesPath() + "shaders/hlsl/" + shaderBaseName + "_ps.cso";

    D3DReadFileToBlob(std::wstring(filePathVS.begin(), filePathVS.end()).c_str(), &vsBlob);
    D3DReadFileToBlob(std::wstring(filePathPS.begin(), filePathPS.end()).c_str(), &psBlob);

    return;

    // DEPRECATED: Compile from hlsl source files.

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
