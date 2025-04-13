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
