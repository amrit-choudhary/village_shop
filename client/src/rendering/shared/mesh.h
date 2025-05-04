/**
 * Mesh class for rendering 3D objects.
 */

#pragma once

#include <stdint.h>

#include <vector>

#include "src/math/vec2.h"
#include "src/math/vec3.h"
#include "src/math/vec4.h"
#include "vertex.h"

namespace ME {
/**
 *
 */
class Mesh {
   public:
    Mesh();
    ~Mesh();

    void LoadFromFile(const char* filename);
    void Render();
    void SetPosition(float x, float y, float z);
    void SetRotation(float x, float y, float z);
    void SetScale(float x, float y, float z);

    void CalculateNormal();

    std::vector<ME::Vertex> vertices;  // List of vertices that make up the mesh
    std::vector<uint32_t> indices;     // List of indices for indexed drawing

    uint32_t vertexCount = 0;
    uint32_t indexCount = 0;
};

}  // namespace ME
