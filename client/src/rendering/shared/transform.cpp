#include "transform.h"

ME::Transform::Transform() {
    position = ME::Vec3(0.0f, 0.0f, 0.0f);
    rotation = ME::Vec4(0.0f, 0.0f, 0.0f, 1.0f);
    scale = ME::Vec3(1.0f, 1.0f, 1.0f);
}

ME::Transform::~Transform() {}

void ME::Transform::SetPosition(float x, float y, float z) {
    position.x = x;
    position.y = y;
    position.z = z;
}

void ME::Transform::SetRotation(float x, float y, float z, float w) {
    rotation.x = x;
    rotation.y = y;
    rotation.z = z;
    rotation.w = w;
}

void ME::Transform::SetScale(float x, float y, float z) {
    scale.x = x;
    scale.y = y;
    scale.z = z;
}
