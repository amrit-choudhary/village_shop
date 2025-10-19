#pragma once
#ifdef VG_WIN

/**
 * DirectX's specific pipeline state object (PSO) class.
 */

#include <d3d12.h>

#include "shader_directx.h"

namespace ME {

class PSODirectX {
   public:
    PSODirectX();
    ~PSODirectX();

    /** Create PSO for 3D rendering */
    static void CreatePSO(ID3D12Device* device, const char* shaderName);

    /** Create PSO for 2D rendering */
    static void CreatePSO2D(ID3D12Device* device, const char* shaderName, ID3D12PipelineState* pso);

   private:
};

}  // namespace ME

#endif  // VG_WIN
