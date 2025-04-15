#include "mesh.h"

ME::Mesh::Mesh() {
    // Initialize position, rotation, and scale to identity
    position = ME::Math::Vec4{0.0f, 0.0f, 0.0f, 1.0f};
    rotation = ME::Math::Vec4{0.0f, 0.0f, 0.0f, 1.0f};
    scale = ME::Math::Vec4{1.0f, 1.0f, 1.0f, 1.0f};
}

ME::Mesh::~Mesh() {}

void ME::Mesh::LoadFromFile(const char* filename) {}

void ME::Mesh::Render() {}

void ME::Mesh::SetPosition(float x, float y, float z) {
    position.x = x;
    position.y = y;
    position.z = z;
    position.w = 1.0f;  // Homogeneous coordinate
}

void ME::Mesh::SetRotation(float x, float y, float z) {
    rotation.x = x;
    rotation.y = y;
    rotation.z = z;
    rotation.w = 1.0f;  // Homogeneous coordinate
}

void ME::Mesh::SetScale(float x, float y, float z) {
    scale.x = x;
    scale.y = y;
    scale.z = z;
    scale.w = 1.0f;  // Homogeneous coordinate
}

void ME::Mesh::CalculateNormal() {
    for (int i = 0; i < indices.size() - 3; i += 3) {
        int vertIndex = indices[i];
        ME::Math::Vec3& v0 = vertices[indices[i]].position;
        ME::Math::Vec3& v1 = vertices[indices[i + 1]].position;
        ME::Math::Vec3& v2 = vertices[indices[i + 2]].position;
        ME::Math::Vec3 normal = ME::Math::Vec3::Cross(v1 - v0, v2 - v0);
        normal.Normalise();
        vertices[vertIndex].normal = normal;
    }
}
