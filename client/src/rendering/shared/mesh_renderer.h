/**
 * MeshRenderer class
 * This class is responsible for rendering 3D meshes.
 * It contains information about the mesh, transform and material.
 */

#pragma once

#include <cstdint>

#include "color.h"
#include "transform.h"

namespace ME {

class MeshRenderer {
   public:
    MeshRenderer() = delete;
    MeshRenderer(uint8_t meshId, uint8_t materialId, uint8_t textureId, const Color& color)
        : meshId(meshId), materialId(materialId), textureId(textureId), color(color) {}
    ~MeshRenderer();

    const uint8_t meshId = 0;
    const uint8_t materialId = 0;
    const uint8_t textureId = 0;
    const ME::Color color;
};

}  // namespace ME
