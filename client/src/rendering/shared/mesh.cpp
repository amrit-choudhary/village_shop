#include "mesh.h"

ME::Mesh::Mesh() {}

ME::Mesh::~Mesh() {}

void ME::Mesh::LoadFromFile(const char* filename) {}

void ME::Mesh::Render() {}

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
