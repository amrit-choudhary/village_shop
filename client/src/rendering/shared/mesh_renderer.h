/**
 * MeshRenderer class
 * This class is responsible for rendering 3D meshes.
 * It contains information about the mesh, transform and material.
 */

#pragma once

#include <cstdint>

#include "transform.h"

namespace ME {

class MeshRenderer {
   public:
    MeshRenderer() = delete;
    ~MeshRenderer();
    MeshRenderer(uint8_t meshId, uint8_t materialId, uint8_t textureId, const Transform& transform)
        : meshId(meshId), materialId(materialId), textureId(textureId), transform(transform) {}

    const uint8_t meshId = 0;
    const uint8_t materialId = 0;
    const uint8_t textureId = 0;
    ME::Transform transform;
};

}  // namespace ME
