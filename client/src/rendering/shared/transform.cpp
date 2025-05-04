#include "transform.h"

ME::Transform::Transform() {}

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
